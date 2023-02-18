#include "vst_plugin.hpp"

using namespace Steinberg;

namespace live::tritone::vie::vst {
	const FUID vst_plugin::processor_uid(0xbc00f531, 0x19a89841, 0x612dcc8c, 0x45bbceed);
	const FUID vst_plugin::controller_uid(0x2afcd1cb, 0x6defd15e, 0x239a1b9e, 0x86959eaf);
}