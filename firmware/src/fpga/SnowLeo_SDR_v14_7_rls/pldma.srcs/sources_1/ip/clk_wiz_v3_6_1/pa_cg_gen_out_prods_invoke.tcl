# Tcl script generated by PlanAhead

set reloadAllCoreGenRepositories false

set tclUtilsPath "d:/XILINX/14.4/ISE_DS/PlanAhead/scripts/pa_cg_utils.tcl"

set repoPaths ""

set cgIndexMapPath "F:/ZYNQ/ZingNano/Zingnano_test/SNOWLeo_Sdr_Demo/SNOWLeo_Sdr_Demo/pldma.srcs/sources_1/ip/cg_nt_index_map.xml"

set cgProjectPath "f:/ZYNQ/ZingNano/Zingnano_test/SNOWLeo_Sdr_Demo/SNOWLeo_Sdr_Demo/pldma.srcs/sources_1/ip/clk_wiz_v3_6_1/coregen.cgc"

set ipFile "f:/ZYNQ/ZingNano/Zingnano_test/SNOWLeo_Sdr_Demo/SNOWLeo_Sdr_Demo/pldma.srcs/sources_1/ip/clk_wiz_v3_6_1/clk_wiz_v3_6_1.xco"

set ipName "clk_wiz_v3_6_1"

set hdlType "Verilog"

set cgPartSpec "xc7z010-1clg400"

set chains "GENERATE_CURRENT_CHAIN"

set params ""

set bomFilePath "f:/ZYNQ/ZingNano/Zingnano_test/SNOWLeo_Sdr_Demo/SNOWLeo_Sdr_Demo/pldma.srcs/sources_1/ip/clk_wiz_v3_6_1/pa_cg_bom.xml"

# generate the IP
set result [source "d:/XILINX/14.4/ISE_DS/PlanAhead/scripts/pa_cg_gen_out_prods.tcl"]

exit $result

