Class fileReader reads and extracts datasets from text or vtk file(s), and create C++ vectors of floating point numbers for each dataset.


Class fileWriter takes the results from fileReader or an C++ vector of floating point numbers and write the datasets in Txt or VTK formats.


Class Chi2Test performs chi-squred statistical test. Several results can be computed: chi-squred statistic, p-value, and sum of absolute errors.


Class Visualize allows visualization of a specific data field in given vtk file(s). With different meshes, the visualization result may not be ideal. There are a couple things that may be modified:

In Visualize::visualization():
	renderWindow->SetSize() changes the size of the window;
	camera->SetViewUp() sets the view angle;
	camera->SetPosition() sets camera position;
	camera->SetFocalPoint() changes focal point of the camera;
In Visualize::createRenderInViewPort():
	legend->GetPositionCoordinate()->SetValue() modifies location of the colour bar;
	clip->SetNormal() changes the plane at which the clip is created;
	clip->SetOrigin(##, ##, ##) sets location of the origin.

For a typical use case, check out FullMonte_MMC_Comparison/




******Additional Notes on MATLAB Executable******

1. Verify the MATLAB Runtime is installed and ensure you have installed version 9.2 (R2017a).   

2. If the MATLAB Runtime is not installed, do the following:
  (1) enter
  
      >>mcrinstaller
      
      at MATLAB prompt. The MCRINSTALLER command displays the 
      location of the MATLAB Runtime installer.

  (2) run the MATLAB Runtime installer.

Or download the Linux 64-bit version of the MATLAB Runtime for R2017a 
from the MathWorks Web site by navigating to

   http://www.mathworks.com/products/compiler/mcr/index.html
   
   
For more information about the MATLAB Runtime and the MATLAB Runtime installer, see 
Package and Distribute in the MATLAB Compiler documentation  
in the MathWorks Documentation Center.    

3. To run the shell script, type
   
       ./run_comparePlot.sh <mcr_directory> <argument_list>
       
    at Linux command prompt. <mcr_directory> is the directory 
    where version 9.2 of the MATLAB Runtime is installed or the directory where 
    MATLAB is installed on the machine. <argument_list> is all the 
    arguments you want to pass to your application. For example, 

    If you have version 9.2 of the MATLAB Runtime installed in 
    /mathworks/home/application/v92, run the shell script as:
    
       ./run_comparePlot.sh /mathworks/home/application/v92
       
    If you have MATLAB installed in /mathworks/devel/application/matlab, 
    run the shell script as:
    
       ./run_comparePlot.sh /mathworks/devel/application/matlab


4. The MATLAB executable is compiled and requires to run on Linux x86-64 system. After installed the MATLAB Runtime, replace MCR_ROOT by the directory where the MATLAB Runtime is installed on the target machine.
  (1) Set the environment variable XAPPLRESDIR to this value:

    MCR_ROOT/v92/X11/app-defaults

  (2) If the environment variable LD_LIBRARY_PATH is undefined, set it to the concatenation of the following strings:

    MCR_ROOT/v92/runtime/glnxa64:
    MCR_ROOT/v92/bin/glnxa64:
    MCR_ROOT/v92/sys/os/glnxa64:
    MCR_ROOT/v92/sys/opengl/lib/glnxa64

    If it is defined, set it to the concatenation of these strings:

    ${LD_LIBRARY_PATH}: 
    MCR_ROOT/v92/runtime/glnxa64:
    MCR_ROOT/v92/bin/glnxa64:
    MCR_ROOT/v92/sys/os/glnxa64:
    MCR_ROOT/v92/sys/opengl/lib/glnxa64

   For more detail information about setting the MATLAB Runtime paths, see Package and 
   Distribute in the MATLAB Compiler documentation in the MathWorks Documentation Center.


     
        NOTE: To make these changes persistent after logout on Linux 
              or Mac machines, modify the .cshrc file to include this  
              setenv command.
        NOTE: The environment variable syntax utilizes forward 
              slashes (/), delimited by colons (:).  
        NOTE: When deploying standalone applications, it is possible 
              to run the shell script file run_plotEnergy.sh 
              instead of setting environment variables. See 
              section 2 "Files to Deploy and Package".    






