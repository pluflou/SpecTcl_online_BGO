#  SpecTclGUI save file created Tue Aug 06 17:18:16 EDT 2019
#  SpecTclGui Version: 1.0
#      Author: Ron Fox (fox@nscl.msu.edu)

#Tree params:

catch {treeparameter -create BGO.ecal.00 0 4096 4096 unknown}
treeparameter -setlimits BGO.ecal.00 0 4096
treeparameter -setbins   BGO.ecal.00 4096
treeparameter -setunit   BGO.ecal.00 unknown

catch {treeparameter -create BGO.energy.00 0 4096 4096 unknown}
treeparameter -setlimits BGO.energy.00 0 4096
treeparameter -setbins   BGO.energy.00 4096
treeparameter -setunit   BGO.energy.00 unknown

catch {treeparameter -create BGO.raw.chanid000.adc 0 65536 65536 unknown}
treeparameter -setlimits BGO.raw.chanid000.adc 0 65536
treeparameter -setbins   BGO.raw.chanid000.adc 65536
treeparameter -setunit   BGO.raw.chanid000.adc unknown


# Pseudo parameter definitions


# Tree variable definitions:


# Gate definitions in reverse dependency order
 

# Spectrum Definitions

spectrum BGO_ModID00_ChID00 1 BGO.raw.chanid000.adc {{0.000000 40000.000000 8000}} long
spectrum bgoarray 1 BGOarray.raw.chanid000.adc {{0.000000 65536.000000 65536}} long
spectrum bgonotarray 1 BGO.raw.chanid000.adc {{0.000000 65536.000000 65536}} long

# Gate Applications: 


#  filter definitions: ALL FILTERS ARE DISABLED!!!!!!!


#  - Parameter tab layout: 

set parameter(select) 1
set parameter(Array)  false

#-- Variable tab layout

set variable(select) 1
set variable(Array)  0
