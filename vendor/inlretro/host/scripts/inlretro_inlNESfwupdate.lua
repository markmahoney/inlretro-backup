-- main script that runs application logic and flow


-- initial function called from C main
function main ()


	print("\n")

	local dict = require "scripts.app.dict"
	local fwupdate = require "scripts.app.fwupdate"



	--Firmware update without bootloader
	--fwupdate.get_fw_appver(true)	

	--active development path (based on makefile in use)
	--fwupdate.update_firmware("../firmware/build_stm/inlretro_stm.bin", 0x6DC, false) --INL6 skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stm/inlretro_stm.bin", 0x6E8, false) --INL_NES skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stm/inlretro_stm.bin", nil, true ) --Know what I'm doing? force the update
	
	--released INL6 path (big square boards)
	--fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm_AV00.bin")
	--fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm_AV01.bin", 0x6DC, false) --INL6 skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm_AV02.bin", 0x6DC, false) --INL6 skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm_AV03.bin", 0x6DC, false) --INL6 skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stm6/inlretro_stm.bin",      0x6DC, false) --nightly build
	
	--released INL_N path (smaller NESmaker boards)
	--fwupdate.update_firmware("../firmware/build_stmn/inlretro_stm_AV00.bin")
	--fwupdate.update_firmware("../firmware/build_stmn/inlretro_stm_AV01.bin", 0x6E8, false) --INL_NES skip ram pointer
	fwupdate.update_firmware("../firmware/build_stmn/inlretro_stm_AV03.bin", 0x6E8, false) --INL_NES skip ram pointer
	--fwupdate.update_firmware("../firmware/build_stmn/inlretro_stm.bin",      0x6E8, false) --nightly build
		


end


main ()

