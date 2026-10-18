#pragma once

#include <cstdint>
#include <format>
#include <string>

namespace TheosRenderPipeline::SourceDLSSG
{
	// The RTX 20/30 backport ships no machine code for those GPUs: the driver
	// compiles DLSS-G's PTX when the kernels load. DLSS-G 310.9.1 and 310.9.16
	// contain PTX 8.7 (CUDA 12.8), which older drivers cannot compile; NGX then
	// fails NvAPI_D3D12_CreateCuModule on every creation attempt. The R570
	// branch is the first with CUDA 12.8; 572.16 is its first Game Ready release.
	inline constexpr std::uint32_t kBackportMinimumDriver = 57000;
	inline constexpr std::uint32_t kBackportRecommendedDriver = 57216;

	// NVIDIA's driver number in hundredths (566.36 is 56636) from the low half
	// of the DXGI user-mode driver version 32.0.15.6636: the last digit of the
	// third part followed by the fourth part. 0 when it does not have that form.
	inline std::uint32_t NvidiaDriverVersion(std::uint32_t userModeVersionLow)
	{
		const auto third = userModeVersionLow >> 16, fourth = userModeVersionLow & 0xFFFFu;
		if (!third || fourth > 9999) { return 0; }
		return (third % 10) * 10000 + fourth;
	}

	inline std::string DriverVersionText(std::uint32_t version)
	{
		return std::format("{}.{:02}", version / 100, version % 100);
	}

	// An unknown version is not refused: the driver may still be recent enough.
	inline bool BackportDriverSupported(std::uint32_t version)
	{
		return !version || version >= kBackportMinimumDriver;
	}
}
