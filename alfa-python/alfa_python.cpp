/*  ***************************************************************************
 *   alpha_python.cpp - Demonstrates the usage of ALFA libraries and dataset.
 *
 *   For more information about the dataset, please refer to:
 *   http://theairlab.org/alfa-dataset
 *
 *   For more information about this project and the publications related to
 *   the dataset and this work, please refer to:
 *   http://theairlab.org/fault-detection-project
 *
 *   Air Lab, Robotics Institute, Carnegie Mellon University
 *
 *   Authors: Azarakhsh Keipour, Mohammadreza Mousaei, Sebastian Scherer
 *   Contact: keipour@cmu.edu
 *
 *   Last Modified: April 16, 2019
 *
 *   Copyright (c) 2019 Carnegie Mellon University,
 *   Azarakhsh Keipour <keipour@cmu.edu>
 *
 *   For License information please see the README file in the root directory.
 *
 *   ***************************************************************************/
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>
#include <string>
#include "sequence.h"
#include "commons.h"
#include "topic.h"
#include "message.h"


using namespace boost::python;

bool operator==(const alfa::Topic& left, const alfa::Topic& right)
{
    return left.Name == right.Name;
}

bool operator==(const alfa::Sequence::MessageIndex& left, const alfa::Sequence::MessageIndex& right)
{
    return left.TopicIdx == right.TopicIdx && left.MessageIdx == right.MessageIdx;
}

// Defines a python module which will be named "alfa-python"
BOOST_PYTHON_MODULE(alfa_python)
{
    
    class_<alfa::Topic>("Topic", init<std::string, std::string>())
		// Class Data Members
		.def_readwrite("Name", &alfa::Topic::Name)
		.def_readwrite("FileName", &alfa::Topic::FileName)
		.def_readwrite("Messages", &alfa::Topic::Messages)
		.def_readonly("FieldLabels", &alfa::Topic::FieldLabels)
	  // Member Functions
		.def("ReadFromFile", &alfa::Topic::ReadFromFile)
		.def("Print", &alfa::Topic::Print)
		.def("PrintHeader", &alfa::Topic::PrintHeader)
		.def("IsInitialized", &alfa::Topic::IsInitialized)
		.def("IsFaultTopic", &alfa::Topic::IsFaultTopic)
		.def("HasHeaderField", &alfa::Topic::HasHeaderField)
		.def("FindLabelIndex", &alfa::Topic::FindLabelIndex)
		.def("Clear", &alfa::Topic::Clear)
		.def("GetTimes", &alfa::Topic::GetTimes)
		.def("GetHeaders", &alfa::Topic::GetHeaders)
		.def("GetFieldsAsStringByString", &alfa::Topic::GetFieldsAsStringByString)
		.def("GetFieldsAsStringByIndex", &alfa::Topic::GetFieldsAsStringByIndex)
		.def("GetFieldsAsIntByString", &alfa::Topic::GetFieldsAsIntByString)
		.def("GetFieldsAsIntByIndex", &alfa::Topic::GetFieldsAsIntByIndex)
		.def("GetFieldsAsLongLongByString", &alfa::Topic::GetFieldsAsLongLongByString)
		.def("GetFieldsAsLongLongByIndex", &alfa::Topic::GetFieldsAsLongLongByIndex)
		.def("GetFieldsAsDoubleByString", &alfa::Topic::GetFieldsAsDoubleByString)
		.def("GetFieldsAsDoubleByIndex", &alfa::Topic::GetFieldsAsDoubleByIndex)
		.def("GetFieldsAsLongDoubleByString", &alfa::Topic::GetFieldsAsLongDoubleByString)
		.def("GetFieldsAsLongDoubleByIndex", &alfa::Topic::GetFieldsAsLongDoubleByIndex)
		;

	class_<alfa::Sequence>("Sequence", init<std::string, std::string>())
		// Class Data Members
		.def_readwrite("Name", &alfa::Sequence::Name)
		.def_readwrite("DirectoryPath", &alfa::Sequence::DirectoryPath)
		.def_readonly("Topics", &alfa::Sequence::Topics)
		.def_readonly("MessageIndexList", &alfa::Sequence::MessageIndexList)
	  // Member Functions
		.def("LoadSequence", &alfa::Sequence::LoadSequence)
	  .def("IsInitialized", &alfa::Sequence::IsInitialized)
	  .def("Clear", &alfa::Sequence::Clear)
	  .def("GetMessage", &alfa::Sequence::GetMessage)
	  .def("PrintBriefInfo", &alfa::Sequence::PrintBriefInfo)
	  .def("GetFaultTopics", &alfa::Sequence::GetFaultTopics)
	  .def("GetTotalDuration", &alfa::Sequence::GetTotalDuration)
	  .def("GetNormalFlightDuration", &alfa::Sequence::GetNormalFlightDuration)
	  .def("FindFirstFaultMessage", &alfa::Sequence::FindFirstFaultMessage)
	  .def("FindTopicIndex", &alfa::Sequence::FindTopicIndex)
		;

	class_<alfa::Sequence::MessageIndex>("MessageIndex", init<int, int>())
		.def_readwrite("TopicIdx", &alfa::Sequence::MessageIndex::TopicIdx)
		.def_readwrite("MessageIdx", &alfa::Sequence::MessageIndex::MessageIdx)
		;

	class_<alfa::Message>("Message")
		.def_readwrite("DateTime", &alfa::Message::DateTime)
		.def_readwrite("Header", &alfa::Message::Header)
		.def_readwrite("Fields", &alfa::Message::Fields)
		;

	class_<alfa::DateTime>("DateTime")
		.def_readwrite("Year", &alfa::DateTime::Year)
		.def_readwrite("Month", &alfa::DateTime::Month)
		.def_readwrite("Day", &alfa::DateTime::Day)
		.def_readwrite("Hour", &alfa::DateTime::Hour)
		.def_readwrite("Minute", &alfa::DateTime::Minute)
		.def_readwrite("Second", &alfa::DateTime::Second)
		.def_readwrite("Nanosecond", &alfa::DateTime::Nanosecond)
		.def("ToString", &alfa::DateTime::ToString)
		.def(self - self)
		;

	class_<std::vector<alfa::Topic>>("VectorTopic")
        .def(vector_indexing_suite<std::vector<alfa::Topic>>());

	class_<std::vector<alfa::Sequence::MessageIndex>>("VectorMessageIndex")
        .def(vector_indexing_suite<std::vector<alfa::Sequence::MessageIndex>>());

	class_<std::vector<double>>("VectorDouble")
        .def(vector_indexing_suite<std::vector<double>>());

	class_<std::vector<alfa::DateTime>>("VectorDateTime")
        .def(vector_indexing_suite<std::vector<alfa::DateTime>>());

	// class_<alfa::Commons>("Commons")
	// 	// Class Data Members
	// 	.def_readonly("CSVDelimiter", &alfa::Commons::CSVDelimiter)
	// 	.def_readonly("CSVFileExtension", &alfa::Commons::CSVFileExtension)
	// 	.def_readonly("FilePathSeparator", &alfa::Commons::FilePathSeparator)
	// 	.def_readonly("CSVDateTimeFormat", &alfa::Commons::CSVDateTimeFormat)
	// 	.def_readonly("CSVFieldsPrefix", &alfa::Commons::CSVFieldsPrefix)
	// 	.def_readonly("FaultTopicPrefix", &alfa::Commons::FaultTopicPrefix)
	// 	// Member Functions
	// 	.def("Tokenize", &alfa::Commons::Tokenize)
	// 	.def("StringToInt", &alfa::Commons::StringToInt)
	// 	.def("StringToLong", &alfa::Commons::StringToLong)
	// 	.def("StringToLongLong", &alfa::Commons::StringToLongLong)
	// 	.def("StringToDouble", &alfa::Commons::StringToDouble)
	// 	.def("StringToLongDouble", &alfa::Commons::StringToLongDouble)
	// 	.def("GetFileList", &alfa::Commons::GetFileList)
	// 	.def("FilterFileList", &alfa::Commons::FilterFileList)
	// 	.def("ExtractFilenameAndExtension", &alfa::Commons::ExtractFilenameAndExtension)
	// 	;
}
