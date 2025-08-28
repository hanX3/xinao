import nptdms
from nptdms import TdmsFile

import ROOT
import numpy as np
from array import array

from tqdm import tqdm

import argparse

#
DATA_DIR = "/mnt/e/11Bpg/data/5160_data/Am241-gamma/Test_+PL_00030/"
GP = "DAQ Data"
CH = "CH01"

#
def read_tdms_file(filename):
    try:
        tdms_file = TdmsFile.read(filename)
        return tdms_file
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
def tdms2tree_sic(time_str, data, props):
    fo = ROOT.TFile(f"{DATA_DIR}../rootfile/{time_str}.root", "RECREATE")
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
    for i in tqdm(range(n_entry), desc="Filling Tree", unit="entry"):
        entry = data[i * length : (i + 1) * length]
        for j in range(length):
            w[j] = entry[j]
        tr.Fill()

    tr.Write()
    fo.Close()
    print(f"Tree saved to {time_str}.root with {n_entry} entries.")

#
def tdms2tree_axuv(time_str, data, props):
    fo = ROOT.TFile(f"{DATA_DIR}../rootfile/{time_str}.root", "RECREATE")
    length = props['wf_samples']

    print(f"length {length}")

    tr = ROOT.TTree(f"tr", f"{length}")

    w = array('h', [0]*length)
    tr.Branch("w", w, f"w[{length}]/S")

    n_entry = len(data) // length
    for i in tqdm(range(n_entry), desc="Filling Tree", unit="entry"):
        entry = data[i * length : (i + 1) * length]
        for j in range(length):
            w[j] = entry[j]
        tr.Fill()

    tr.Write()
    fo.Close()
    print(f"Tree saved to {time_str}.root with {n_entry} entries.")

#
def tdms2tree_gagg(time_str, data, props):
    fo = ROOT.TFile(f"{DATA_DIR}../../../../rootfile/{time_str}.root", "RECREATE")
    length = props['wf_samples']

    print(f"length {length}")

    tr = ROOT.TTree(f"tr", f"{length}")

    w = array('h', [0]*length)
    tr.Branch("w", w, f"w[{length}]/S")

    n_entry = len(data) // length
    for i in tqdm(range(n_entry), desc="Filling Tree", unit="entry"):
        entry = data[i * length : (i + 1) * length]
        for j in range(length):
            w[j] = int(entry[j])
        tr.Fill()

    tr.Write()
    fo.Close()
    print(f"Tree saved to {time_str}.root with {n_entry} entries.")

##
def main(data_name):
    print(f"Processing: {data_name}")

    tdms_file_name = f"{DATA_DIR}{data_name}.tdms"
    print(f"{tdms_file_name}")
    tdms_file  = read_tdms_file(tdms_file_name)
    if tdms_file is None:
        return

    data_dict, props_dict = extract_channel_data(tdms_file[GP])
    data = data_dict[CH]
    props = props_dict[CH]
    tdms2tree_gagg(data_name, data, props)

##
if __name__ == "__main__":
    par = argparse.ArgumentParser(description="Process data by name.")
    par.add_argument("data_name", help="The name of the data to process")
    args = par.parse_args()
    
    main(args.data_name)

