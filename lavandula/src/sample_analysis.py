import os
import subprocess

try:
    import pyext
except:
    print("ERROR: Must load from pyext external in Pd")

"""
Sample collection updater (for Musly collection)
"""
class collection_updater(pyext._class):
    # Number of inlets and outlets
    _inlets = 3
    _outlets = 1

    # Instance variables
    _prev_filename = ""
    _cur_filename = ""
    _session_path = ""
    _collection_path = ""
    # _collection_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/collection_recorded.musly"
    _collection_default_name = "collection.musly"
    _collection_method = "mandelellis" # "mandelellis" or "timbre"
    _samples_base_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/recorded"
    _musly_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/lib/musly"

    def bang_1(self):
        if len(self._prev_filename) > 0:
            sample_path = os.path.join(self._session_path, self._prev_filename)
            cmd_add = "{} -a {} -c {}".format(self._musly_path, sample_path, self._collection_path)
            # print("cmd_add: {}".format(cmd_add))
            # cmd_playlist = "musly -p {} -c {}".format(self._collection_path, self._samples_path)
            result = subprocess.check_output(cmd_add.split(" "))
            print(result)
            if "[OK]" in result:
                print("Added prev file {} to collection".format(self._prev_filename))
            else:
                print("Failed to add file {} to collection".format(self._prev_filename))

    # Update prev and cur filenames based on new recordings triggered
    def _anything_2(self, *args):
        self._prev_filename = self._cur_filename
        self._cur_filename = str(args[0])

    # Update session, samples, and collection paths
    def _anything_3(self, *args):
        # Construct new session path and create dir if necessary
        cur_session_path = str(args[0])
        self._session_path = os.path.join(self._samples_base_path, cur_session_path)
        if not os.path.exists(self._session_path):
            try:
                os.makedirs(self._session_path)
            except OSError as exception:
                print("Caught OSError when trying to create {}!".format(self._session_path))
        
        # Initialize new collection
        self._collection_path = os.path.join(self._session_path, self._collection_default_name)
        cmd = "{} -n {} -c {}".format(self._musly_path, self._collection_method, self._collection_path)
        result = subprocess.check_output(cmd.split(" "))
        print(result)