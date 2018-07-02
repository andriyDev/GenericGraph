#pragma once

#include "OFSM.h"
#include "OFSMNode.h"
#include "OFSMEdge.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "IOFSMEditor.h"

#define LOG_INFO(FMT, ...) UE_LOG(OFSMEditor, Display, (FMT), ##__VA_ARGS__)
#define LOG_WARNING(FMT, ...) UE_LOG(OFSMEditor, Warning, (FMT), ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) UE_LOG(OFSMEditor, Error, (FMT), ##__VA_ARGS__)
