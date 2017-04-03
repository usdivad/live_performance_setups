import os
import shutil
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
    _inlets = 4
    _outlets = 1

    # Instance variables
    _prev_filename = ""
    _cur_filename = ""
    _session_path = ""
    _similarity_matrix_path = ""
    _similarity_matrix_name = "similarity_matrix.mirex"
    _collection_path = ""
    _collection_name = "collection.musly"
    _collection_method = "mandelellis" # "mandelellis" or "timbre"
    _collection_template_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/collection_templates/cpaghetti"
    _samples_base_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/recorded"
    _musly_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/lib/musly"
    _analysis_method = 0 # 0 = use similarity matrix, 1 = use collection templates

    # Analyze prev sample
    def bang_1(self):
        print("Analysis mode: {}".format(self._analysis_method))
        if len(self._prev_filename) > 0:
            sample_path = os.path.join(self._session_path, self._prev_filename)
            if self._analysis_method == 0:
                cmd_add = "{} -a {} -c {}".format(self._musly_path, sample_path, self._collection_path)
                result = subprocess.check_output(cmd_add.split(" "))
                # print(result)
                if "[OK]" in result:
                    print("Added prev file {} to collection".format(self._prev_filename))
                    cmd_similarity = "{} -m {} -c {}".format(self._musly_path, self._similarity_matrix_path, self._collection_path)
                    result = subprocess.check_output(cmd_similarity.split(" "))
                    print(result)
                elif "Skipping" not in result:
                    print("Failed to add prev file {} to collection".format(self._prev_filename))
            elif self._analysis_method == 1:
                for root, dirs, filenames in os.walk(self._session_path):
                    num_collections = 0
                    num_successful_additions = 0
                    for filename in filenames:
                        if filename.endswith("musly"):
                            num_collections += 1
                            collection_path = os.path.join(self._session_path, filename)
                            cmd_add = "{} -a {} -c {}".format(self._musly_path, sample_path, collection_path)
                            result = subprocess.check_output(cmd_add.split(" "))
                            # print(result)
                            if "[OK]" in result:
                                num_successful_additions += 1
                    if num_successful_additions > 0:
                        print("Added prev file {} to {} of {} collections".format(self._prev_filename, num_successful_additions, num_collections))
                    elif "Skipping" not in result:
                        print("Failed to add prev file {} to any collections".format(self._prev_filename))
            else:
                print("Invalid analysis method {}".format(self._analysis_method))


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
        
        if self._analysis_method == 0:
            # Initialize new collection
            self._collection_path = os.path.join(self._session_path, self._collection_name)
            self._similarity_matrix_path = os.path.join(self._session_path, self._similarity_matrix_name)
            cmd = "{} -n {} -c {}".format(self._musly_path, self._collection_method, self._collection_path)
            result = subprocess.check_output(cmd.split(" "))
            print(result)
        elif self._analysis_method == 1:
            for root, dirs, filenames in os.walk(self._collection_template_path):
                for filename in filenames:
                    if filename.endswith("musly"):
                        shutil.copy(os.path.join(root, filename), os.path.join(self._session_path, filename))
                        print("Copied {} to {}".format(filename, self._session_path))
        else:
            print("Invalid analysis method {}".format(self._analysis_method))

    # Update analysis method
    def int_4(self, a):
        self._analysis_method = a
        print("Analysis method is now {}".format(a))
