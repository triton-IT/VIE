message(STATUS "VIE> VST> Fetch")

include(FetchContent)

FetchContent_Declare(
    vst_interfaces
    GIT_REPOSITORY https://github.com/steinbergmedia/vst3_pluginterfaces
    GIT_TAG        v3.7.4_build_25
)

FetchContent_GetProperties(vst_interfaces)
if(NOT vst_interfaces_POPULATED)
	FetchContent_Populate(vst_interfaces)
endif()

#Because VST SDK as "pluginterfaces" hardcoded, we need to move sources a folder named "pluginterfaces"...
set(VST_H_PATH ${PROJECT_BINARY_DIR}/vst_interfaces)
file(MAKE_DIRECTORY ${VST_H_PATH}/pluginterfaces)
file(COPY ${vst_interfaces_SOURCE_DIR}/ DESTINATION ${VST_H_PATH}/pluginterfaces)

# base interfaces
add_library(vst_interfaces STATIC
    ${VST_H_PATH}/pluginterfaces/base/conststringtable.cpp
    ${VST_H_PATH}/pluginterfaces/base/conststringtable.h
    ${VST_H_PATH}/pluginterfaces/base/coreiids.cpp
    ${VST_H_PATH}/pluginterfaces/base/falignpop.h
    ${VST_H_PATH}/pluginterfaces/base/falignpush.h
    ${VST_H_PATH}/pluginterfaces/base/fplatform.h
    ${VST_H_PATH}/pluginterfaces/base/fstrdefs.h
    ${VST_H_PATH}/pluginterfaces/base/ftypes.h
    ${VST_H_PATH}/pluginterfaces/base/funknown.cpp
    ${VST_H_PATH}/pluginterfaces/base/funknown.h
    ${VST_H_PATH}/pluginterfaces/base/funknownimpl.h
    ${VST_H_PATH}/pluginterfaces/base/futils.h
    ${VST_H_PATH}/pluginterfaces/base/fvariant.h
    ${VST_H_PATH}/pluginterfaces/base/geoconstants.h
    ${VST_H_PATH}/pluginterfaces/base/ibstream.h
    ${VST_H_PATH}/pluginterfaces/base/icloneable.h
    ${VST_H_PATH}/pluginterfaces/base/ierrorcontext.h
    ${VST_H_PATH}/pluginterfaces/base/ipersistent.h
    ${VST_H_PATH}/pluginterfaces/base/ipluginbase.h
    ${VST_H_PATH}/pluginterfaces/base/istringresult.h
    ${VST_H_PATH}/pluginterfaces/base/iupdatehandler.h
    ${VST_H_PATH}/pluginterfaces/base/keycodes.h
    ${VST_H_PATH}/pluginterfaces/base/pluginbasefwd.h
    ${VST_H_PATH}/pluginterfaces/base/smartpointer.h
    ${VST_H_PATH}/pluginterfaces/base/typesizecheck.h
    ${VST_H_PATH}/pluginterfaces/base/ucolorspec.h
    ${VST_H_PATH}/pluginterfaces/base/ustring.cpp
    ${VST_H_PATH}/pluginterfaces/base/ustring.h
)

# vst3 interfaces
target_sources(vst_interfaces
	PRIVATE
		${VST_H_PATH}/pluginterfaces/gui/iplugview.h
		${VST_H_PATH}/pluginterfaces/gui/iplugviewcontentscalesupport.h
		${VST_H_PATH}/pluginterfaces/vst/ivstattributes.h
		${VST_H_PATH}/pluginterfaces/vst/ivstaudioprocessor.h
		${VST_H_PATH}/pluginterfaces/vst/ivstautomationstate.h
		${VST_H_PATH}/pluginterfaces/vst/ivstchannelcontextinfo.h
		${VST_H_PATH}/pluginterfaces/vst/ivstcomponent.h
		${VST_H_PATH}/pluginterfaces/vst/ivstcontextmenu.h
		${VST_H_PATH}/pluginterfaces/vst/ivsteditcontroller.h
		${VST_H_PATH}/pluginterfaces/vst/ivstevents.h
		${VST_H_PATH}/pluginterfaces/vst/ivsthostapplication.h
		${VST_H_PATH}/pluginterfaces/vst/ivstinterappaudio.h
		${VST_H_PATH}/pluginterfaces/vst/ivstmessage.h
		${VST_H_PATH}/pluginterfaces/vst/ivstmidicontrollers.h
		${VST_H_PATH}/pluginterfaces/vst/ivstmidilearn.h
		${VST_H_PATH}/pluginterfaces/vst/ivstnoteexpression.h
		${VST_H_PATH}/pluginterfaces/vst/ivstparameterchanges.h
		${VST_H_PATH}/pluginterfaces/vst/ivstparameterfunctionname.h
		${VST_H_PATH}/pluginterfaces/vst/ivstphysicalui.h
		${VST_H_PATH}/pluginterfaces/vst/ivstpluginterfacesupport.h
		${VST_H_PATH}/pluginterfaces/vst/ivstplugview.h
		${VST_H_PATH}/pluginterfaces/vst/ivstprefetchablesupport.h
		${VST_H_PATH}/pluginterfaces/vst/ivstprocesscontext.h
		${VST_H_PATH}/pluginterfaces/vst/ivstrepresentation.h
		${VST_H_PATH}/pluginterfaces/vst/ivstunits.h
		${VST_H_PATH}/pluginterfaces/vst/vstpresetkeys.h
		${VST_H_PATH}/pluginterfaces/vst/vstpshpack4.h
		${VST_H_PATH}/pluginterfaces/vst/vstspeaker.h
		${VST_H_PATH}/pluginterfaces/vst/vsttypes.h
)

message(STATUS "VIE> VST> Include directory: " ${VST_H_PATH}/pluginterfaces)
target_include_directories(vst_interfaces PUBLIC
    ${VST_H_PATH}
)

message(STATUS "VIE> VST> Link directory: " ${vst_interfaces_BINARY_DIR})
target_link_directories(vst_interfaces INTERFACE ${vst_interfaces_BINARY_DIR})