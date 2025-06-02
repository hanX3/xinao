import nptdms
from nptdms import TdmsFile

import ROOT
import numpy as np
from array import array

#
DATA_DIR = "/mnt/c/Users/hanX/OneDrive/works/11Bpg/dpp/data/"
GP = "DAQ Data"
CH = "CH01"

#
def read_tdms_file(filename):
    time_str = filename.split('/')[-2].split('_')[1]
    try:
        tdms_file = TdmsFile.read(filename)
        return tdms_file, time_str
    except Exception as e:
        print(f"can not open tdms file {e}")
        return None

#
def extract_channel_data(group):
    data_dict = {}
    props_dict = {}
    length = None

    print("Channels:")
    for ch in group.channels():
        print(f" - {ch.name}")
        values = ch[:]
        if length is None:
            length = len(values)
        elif len(values) != length:
            raise ValueError(f"error diff channel length")
        data_dict[ch.name] = np.array(values)
        props_dict[ch.name] = ch.properties

    return data_dict, props_dict

#
def tdms2tree(time_str, data, props):
    fo = ROOT.TFile(f"./rootfile/{time_str}.root", "RECREATE")
    length = props['Length']
    wf_increment = props['wf_increment']
    duration = length/(wf_increment/1000)

    print(f"length {length}")
    print(f"wf_increment {wf_increment}")
    print(f"duration {duration}")

    wf_increment /= 1000000

    tr = ROOT.TTree(f"tr", f"{wf_increment}MB_{duration}ms")

    w = array('h', [0]*length)
    tr.Branch("w", w, f"w[{length}]/S")

    n_entry = len(data) // length
    for i in range(n_entry):
        entry = data[i * length : (i + 1) * length]
        for j in range(length):
            w[j] = entry[j]
        tr.Fill()

    tr.Write()
    fo.Close()
    print(f"Tree saved to {time_str}.root with {n_entry} entries.")

##
def main(data_name):
    tdms_file_name = f"{DATA_DIR}{data_name}/{data_name}.tdms"
    tdms_file, time_str = read_tdms_file(tdms_file_name)
    if tdms_file is None:
        return

    data_dict, props_dict = extract_channel_data(tdms_file[GP])
    data = data_dict[CH]
    props = props_dict[CH]
    tdms2tree(time_str, data, props)
  
##
if __name__ == "__main__":
    data_name = "Date_2025520130306"
    main(data_name)

