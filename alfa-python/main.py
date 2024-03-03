#  ***************************************************************************
#   main.cpy - Demonstrates the usage of ALFA libraries and dataset.
#
#   For more information about the dataset, please refer to:
#   http://theairlab.org/alfa-dataset
#
#   For more information about this project and the publications related to
#   the dataset and this work, please refer to:
#   http://theairlab.org/fault-detection-project
#
#   Air Lab, Robotics Institute, Carnegie Mellon University
#
#   Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
#   Contact: keipour@cmu.edu
#
#   Last Modified: April 16, 2019
#
#   Copyright (c) 2019 Carnegie Mellon University,
#   Azarakhsh Keipour <keipour@cmu.edu>
#
#   For License information please see the README file in the root directory.
#
#   ***************************************************************************
from alfa_python import Sequence
from alfa_python import Topic
import sys
import os

def extract_filename_and_extension(path):
    # 分割文件路径和扩展名
    base_name = os.path.basename(path)
    dir_name = os.path.dirname(path)
    file_name, extension = os.path.splitext(base_name)
    return dir_name, file_name, extension.lstrip('.'), bool(base_name and dir_name)

def parse_command_line():
    out_sequence_path = ""
    out_sequence_name = ""
    
    # 检查命令行参数数量
    if len(sys.argv) != 2 or sys.argv[-1].startswith('-'):
        print_help_message()
        return False, out_sequence_path, out_sequence_name

    # 提取路径和序列名
    bag_path = sys.argv[1]
    out_sequence_path, out_sequence_name, extension, extracted = extract_filename_and_extension(bag_path)

    # 检查文件扩展名是否正确
    if not extracted or extension != "bag":
        print_help_message()
        return False, out_sequence_path, out_sequence_name

    # 如果路径不以路径分隔符结束，则添加路径分隔符
    if not out_sequence_path.endswith(os.path.sep):
        out_sequence_path += os.path.sep

    return True, out_sequence_path, out_sequence_name

def print_help_message():
    print("Please provide the path to the sequence bag file!")
    print("Usage (in Linux/Mac):")
    print("./main path/to/sequence/bagfile.bag")
    print("Usage (in Windows):")
    print("main.exe path\\to\\sequence\\bagfile.bag")

def print_project_info():
    print("************************************************************************")
    print("* Thank you for using the ALFA dataset!                                *")
    print("* The following is an example of using the provided alpha-cpp code.    *")
    print("* Please contact us about any questions or to report any bugs.         *")
    print("************************************************************************")
    print("* For more information about the dataset, please refer to:             *")
    print("* http://theairlab.org/alfa-dataset                                    *")
    print("*                                                                      *")
    print("* For more information about this project and the publications related *")
    print("* to the dataset and this work, please refer to:                       *")
    print("* http://theairlab.org/fault-detection-project                         *")
    print("*                                                                      *")
    print("* Air Lab, Robotics Institute, Carnegie Mellon University              *")
    print("*                                                                      *")
    print("* Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer  *")
    print("* Contact: keipour@cmu.edu                                             *")
    print("************************************************************************")

def main():
    # Read the dataset name/path from command-line arguments
    parsed, sequenceDir,sequenceName  = parse_command_line()
    # Exit if the command line is not properly formatted
    if (not parsed):
        return 0
    # Print a message about the project
    print_project_info()
    # Read the sequence from the given directory
    sequence = Sequence(sequenceDir, sequenceName)
    # Check if loading the sequence failed
    if (not sequence.IsInitialized()):
        return 0
    # Print some information about the sequence
    sequence.PrintBriefInfo()
    # Find the index for a sample topic
    mytopic_idx = sequence.FindTopicIndex("mavros-nav_info-roll")
    # Print 10 messages starting from number 20 from this topic
    if (mytopic_idx != -1):
        # print(sequence.Topics)
        print(f"Messages from topic {sequence.Topics[mytopic_idx].Name}:")
        print()
        sequence.Topics[mytopic_idx].Print(20, 10, "|")
        print()
    # Print the info for the first 10 messages in the whole sequence
    print("Info on the first 10 messages in the sequence:")
    for i in range(min(10, len(sequence.MessageIndexList))):
        topic_idx = sequence.MessageIndexList[i].TopicIdx
        print(f"{i:2} | Time: {sequence.GetMessage(i).DateTime.ToString()} | Topic: {sequence.Topics[topic_idx].Name}")
    # Print the first fault message
    fault_msg_idx = sequence.FindFirstFaultMessage()
    fault_topic_idx = sequence.MessageIndexList[fault_msg_idx].TopicIdx
    print(f"The first fault message in the sequence is from '{sequence.Topics[fault_topic_idx].Name}' topic.")
    print(f"The fault happens after {sequence.GetMessage(fault_msg_idx).DateTime - sequence.GetMessage(0).DateTime} seconds.")
    sequence.Topics[fault_topic_idx].Print(0, 1, "|")
    # Retrieve the commanded rolls and its times from 'mavros-nav_info-roll' topic (15 first messages)
    rolltopic_idx = sequence.FindTopicIndex("mavros-nav_info-roll")
    rolls = sequence.Topics[rolltopic_idx].GetFieldsAsDoubleByString("commanded", 0, 15)
    roll_times = sequence.Topics[rolltopic_idx].GetTimes(0, 15)
    print("Printing the commanded rolls for the first 15 messages in 'mavros-nav_info-roll'.")
    for i in range(len(rolls)):
        print(f"{i:2} | Time: {roll_times[i].ToString()} | Commanded Roll : {rolls[i]:.10f}")
    
    
main()









