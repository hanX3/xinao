from nptdms import TdmsFile

tdms_file = TdmsFile.read("/mnt/e/11Bpg/data/5160_data/Am241/Test_+PL_00001/00001_DAQ.tdms")

print("File Properties:")
for key, value in tdms_file.properties.items():
    print(f"  {key}: {value}")

print("Groups:")
for group in tdms_file.groups():
    print(" -", group.name)
print(f"Group '{group.name}' Properties:")
for key, value in group.properties.items():
    print(f"  {key}: {value}")

for group in tdms_file.groups():
    print(f"\nGroup: {group.name}")
    for channel in group.channels():
        length = len(channel)
        print(f"Group: {group}, Channel: {channel.name}, Length: {length}")

        print(f"Channel '{channel.name}' Properties:")
        for key, value in channel.properties.items():
            print(f"  {key}: {value}")

