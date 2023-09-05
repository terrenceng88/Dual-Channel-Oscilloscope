# Component constraints for C:\Users\terre\Documents\CSE 121 Stuff\Project Checkoff\tng305\project\design\Project\Design01.cydsn\TopDesign\TopDesign.cysch
# Project: C:\Users\terre\Documents\CSE 121 Stuff\Project Checkoff\tng305\project\design\Project\Design01.cydsn\Design01.cyprj
# Date: Fri, 04 Jun 2021 19:16:17 GMT
# GraphicLCDIntf_1
# False paths present for both 8 and 16 bits Graphic LCD Interface configurations 
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_0}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_0}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_0}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_0}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_1}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_1}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_1}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_1}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_2}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_2}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_2}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_2}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_3}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_3}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_3}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_3}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_4}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_4}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_4}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_4}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_5}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_5}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_5}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_5}]]} 
   
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_6}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_6}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_6}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_6}]]} 
 
expr {(8) == 8 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd8:Lsb\/p_out_7}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_7}]] : \
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Lsb\/p_out_7}] -to [get_pins {\GraphicLCDIntf_1:LsbReg\/status_7}]]} 
   
# False paths present only for 16 bits Graphic LCD Interface configuration
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_0}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_0}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_1}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_1}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_2}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_2}]] : {}}

expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_3}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_3}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_4}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_4}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_5}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_5}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_6}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_6}]] : {}}
   
expr {(8) == 16 ?
   [set_false_path -from [get_pins {\GraphicLCDIntf_1:GraphLcd16:Msb\/p_out_7}] -to [get_pins {\GraphicLCDIntf_1:GraphLcd16:MsbReg\/status_7}]] : {}}

