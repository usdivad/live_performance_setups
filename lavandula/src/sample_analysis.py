import json
import os
import re
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
    _inlets = 6
    _outlets = 1

    # Instance variables
    _prev_filename = ""
    _cur_filename = ""

    _similarity_matrix_path = ""
    _similarity_matrix_name = "similarity_matrix.mirex"
    
    _mapping_prefix = "mappings"
    _mapping_extension = "mmm"
    _num_recent_mappings = 10 # Number of most samples to map
    _num_assignments_per_mapping = 5 # Number of samples to assign per mapping
    _should_map_samples = True # Whether to map samples upon new sample collect
    
    _collection_path = ""
    _collection_name = "collection.musly"
    _collection_method = "mandelellis" # "mandelellis" or "timbre"
    _collection_template_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/collection_templates/cpaghetti"
    
    _session_path = ""
    _samples_base_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/recorded"
    _musly_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/lib/musly"
    
    _analysis_method = 0 # 0 = use similarity matrix, 1 = use collection templates
    _num_samples_collected = 0

    # Analyze prev sample
    def bang_1(self):
        print("Analysis mode: {}".format(self._analysis_method))
        if len(self._prev_filename) > 0:
            sample_path = os.path.join(self._session_path, self._prev_filename)
            if self._analysis_method == 0: # Use similarity matrix
                cmd_add = "{} -a {} -c {}".format(self._musly_path, sample_path, self._collection_path)
                result = subprocess.check_output(cmd_add.split(" "))
                # print(result)
                if "[OK]" in result:
                    # Create similarity matrix
                    self._num_samples_collected += 1
                    print("Added prev file {} to collection".format(self._prev_filename))
                    cmd_similarity = "{} -m {} -c {}".format(self._musly_path, self._similarity_matrix_path, self._collection_path)
                    result = subprocess.check_output(cmd_similarity.split(" "))
                    print(result)

                    # Map samples
                    if self._should_map_samples:
                        self.map_samples()
                    else:
                        print("Skipping sample mapping")

                elif "Skipping" not in result:
                    print("Failed to add prev file {} to collection".format(self._prev_filename))
            elif self._analysis_method == 1: # Use collection templates
                # Add sample to all collection templates in session
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
                        self._num_samples_collected += 1
                        print("Added prev file {} to {} of {} collections".format(self._prev_filename, num_successful_additions, num_collections))
                    elif "Skipping" not in result:
                        print("Failed to add prev file {} to any collections".format(self._prev_filename))

                    # TODO: Update mappings
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
            #self._mappings_path = os.path.join(self._session_path, self._mappings_name)
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

    # Update whether to map samples
    def int_5(self, m):
        self._should_map_samples = True if m > 0 else False
        print("Should map samples? {}".format(self._should_map_samples))

    def bang_6(self):
        self.map_samples()

    ################################################################

    # Update mappings (assignments)
    def map_samples(self):
        print("Updating sample mappings...")

        # Parse similarity matrix
        lines = []
        sample_names = []
        sample_mappings = []

        with open(self._similarity_matrix_path, "r") as f:
            lines = f.readlines()

        if len(lines) < 1:
            print("ERROR: Empty similarity matrix file {}".format(self._similarity_matrix_path))
            return
        lines.pop(0) # Get rid of info header line

        # Get sample names
        # num_samples = self._num_samples_collected
        # for i in range(num_samples):
        has_additional_samples = True
        p = re.compile(r"^\d+\s+")
        while has_additional_samples:
            sample_line = lines.pop(0)
            sample_name = p.sub("", sample_line)
            sample_name = sample_name.replace("\n", "")
            sample_names.append(sample_name) # Sample path, really
            if len(lines)<1 or re.match(r"^Q/R", lines[0]):
                has_additional_samples = False
        print("{} samples, {} lines remaining: {}".format(len(sample_names), len(lines), sample_names))
        if len(lines) < 1:
            print("ERROR: No matrix data in similarity matrix file {}".format(self._similarity_matrix_path))
            return
        lines.pop(0) # Get rid of matrix header
        p = re.compile(r"\s+")
        for i in range(min(len(sample_names), self._num_recent_mappings)):
            sample_mapping = []
            if len(lines) <= i:
                print("ERROR: No matrix data for samples beyond sample {}".format(i-1))
                return
            line = lines[i]
            similarities = p.split(line)
            if len(similarities) < 3:
                print("Warning: No similarities for sample {}".format(i))
                continue
            similarities.pop(0) # Get rid of index
            similarities.pop(-1)
            print("Similarities: {}".format(similarities))
            similarities_sorted = sorted(similarities)
            similarities_indices = [similarities.index(x) for x in similarities_sorted]
            similarities_indices.pop(0) # Get rid of zero-valued similarity (self)
            print("Similarities indices: {}".format(similarities_indices))
            for j in range(self._num_assignments_per_mapping):
                if j >= len(similarities_indices):
                    print("Warning: Only {} similarities for sample {} ({} wanted)".format(len(similarities_indices), i, self._num_assignments_per_mapping))
                    break
                similarity_idx = similarities_indices[j]
                sample_name = sample_names[similarity_idx]
                sample_mapping.append(sample_name)
            sample_mappings.append(sample_mapping)

        # Write to output file
        for i, sample_mapping in enumerate(sample_mappings):
            mapping_name = "{}_{}.{}".format(self._mapping_prefix, i, self._mapping_extension)
            mapping_path = os.path.join(self._session_path, mapping_name)
            with open(mapping_path, "w") as f:
                f.write("\n".join(sample_mapping))

        # OLD: Write all mappings to single 2D array as JSON
        # with open(self._mappings_path, "w") as f:
        #     f.write(json.dumps(sample_mappings))
        #     print("Wrote sample mappings to {}".format(self._mappings_path))
