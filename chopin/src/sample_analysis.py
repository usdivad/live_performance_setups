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
    _inlets = 2
    _outlets = 1

    # Instance variables
    _prev_filename = ""
    _cur_filename = ""
    _collection_path = "/Users/usdivad/Documents/music/live_performance_setups/chopin/data/collection_recorded.musly"
    _samples_path = "/Users/usdivad/Documents/music/live_performance_setups/chopin/data/recorded"
    _musly_path = "/Users/usdivad/Documents/music/live_performance_setups/chopin/lib/musly"

    def bang_1(self):
        if len(self._prev_filename) > 0:
            sample_path = os.path.join(self._samples_path, self._prev_filename)
            cmd_add = "{} -a {} -c {}".format(self._musly_path, sample_path, self._collection_path)
            # print("cmd_add: {}".format(cmd_add))
            # cmd_playlist = "musly -p {} -c {}".format(self._collection_path, self._samples_path)
            result = subprocess.check_output(cmd_add.split(" "))
            print(result)
            if "[OK]" in result:
                print("Added prev file {} to collection".format(self._prev_filename))
            else:
                print("Failed to add file {} to collection".format(self._prev_filename))

    def _anything_2(self, *args):
        # Update prev and cur filenames based on new recordings triggered
        self._prev_filename = self._cur_filename
        self._cur_filename = str(args[0])