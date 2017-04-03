import os
import subprocess

"""
Utility script to create a collection template out of samples
"""
if __name__ == "__main__":
    template_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/collection_templates/cpaghetti"
    samples_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/data/misc/cpaghetti_stems_subset"
    musly_path = "/Users/usdivad/Documents/music/live_performance_setups/lavandula/lib/musly"
    collection_method = "mandelellis"

    for root, dirs, filenames in os.walk(samples_path):
        num_successes = 0
        for sample_name in filenames:
            collection_name = "collection_{}".format(sample_name.replace(".wav", ".musly"))
            collection_path = os.path.join(template_path, collection_name)
            sample_path = os.path.join(root, sample_name)
            print(collection_path)

            cmd_init = "{} -n {} -c {}".format(musly_path, collection_method, collection_path)
            result = subprocess.check_output(cmd_init.split(" "))
            print("cmd_init: {}".format(result))

            cmd_add = "{} -a {} -c {}".format(musly_path, sample_path, collection_path)
            result = subprocess.check_output(cmd_add.split(" "))
            print("cmd_add: {}".format(result))
            if "[OK]" in result:
                num_successes += 1
            else:
                os.remove(collection_path)
                print("Failed {} and thus removed it".format(collection_path))
        print("Successfully processed {} of {} samples into collection templates".format(num_successes, len(filenames)))