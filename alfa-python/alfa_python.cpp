#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <iostream>
#include <string>
#include "sequence.h"
#include "commons.h"
#include "topic.h"
#include "message.h"


using namespace boost::python;

// Defines a python module which will be named "alfa-python"
BOOST_PYTHON_MODULE(alfa_python)
{
    // Declares class Bonjour, specifying the constructor, the greet function
    // and declaring msg as an attribute visible from python, which can be queried
		// and set through the C++ get and set functions.

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
		.def("GetFieldsAsStringByName", &alfa::Topic::GetFieldsAsStringByName)
		.def("GetFieldsAsStringByIndex", &alfa::Topic::GetFieldsAsStringByIndex)
		.def("GetFieldsAsIntByName", &alfa::Topic::GetFieldsAsLongLongByName)
		.def("GetFieldsAsIntByIndex", &alfa::Topic::GetFieldsAsIntByIndex)
		.def("GetFieldsAsLongLongByName", &alfa::Topic::GetFieldsAsLongLongByIndex)
		.def("GetFieldsAsLongLongByIndex", &alfa::Topic::GetFieldsAsDoubleByName)
		.def("GetFieldsAsDoubleByName", &alfa::Topic::GetTimes)
		.def("GetFieldsAsDoubleByIndex", &alfa::Topic::GetTimes)
		.def("GetFieldsAsLongDoubleByName", &alfa::Topic::GetTimes)
		.def("GetFieldsAsLongDoubleByIndex", &alfa::Topic::GetTimes)
		;

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
