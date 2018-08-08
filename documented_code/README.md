# PIR-project @ CTAI-PUJ

PIR project was conceived in 2016 as a long-term research initiative at CTAI (Centro Tecnológico de Automatización Industrial), focused on conducting research on the technical aspect of the collaborative and flexibility concepts of Co-ROBOTs in the industry. We aim to create a framework where we will develop the software and hardware required for the human-machine interaction in an industrial environment *(collaborative concept)*; and that provides flexibility to the same robot to work in different tasks, by incorporating perception and intelligence to the robot *(flexibility concept)*.

## Bottle Clasification Task

Currently, many industrial processes in Colombia are still done manually, for instance; the recycling process, particularly the waste separation task. This process is usually done by vulnerable population or low-income families, that are dedicated to collect recyclable materials directly from mixed-waste bins and bring it to a public collection center where it is classified.

The main purpose of this research is to make a first approach to analyze the waste pre-classification task of plastic bottles in Bogotá and propose its automation in an emulated human-robot co-existence workstation. The process is simulated at the Centro Tecnológico de Automatización Industrial (CTAI) from PUJ, where a testbed was created. It consists on an emulated pre-classification workstation where  three types of  thermoplastic recyclable materials, are classified and their 3D pose estimation is obtained using the IR (infrared) depth sensor of the Microsoft kinect. Additionally, the SDA10F dual-arm Motoman robot equipped with a Robotiq gripper is used to perform the pick and place task, carrying the bottles to the appropriate bin. To distribute the designed system two laptops are used; one in charge of running the image processing algorithm, and another one in charge of the environment simulation and controlling of the robot.

The system proposed is based on open source software, using the Robot Operating System (ROS) and the OpenCV libraries for developing the image processing algorithm. The system consists on four stages; the first stage uses the OpenCV libraries to design a computer vision algorithm. The second stage uses the data acquired by the CV algorithm to locate the bottles on the work-table and characterize them it by their type and geometry. The third stage uses the pose and the type of object detected in the previous stage, to plan the robot’s trajectory. The last stage converts all the information into the robot’s commands that the controller of the robot and the controller of the gripper will interpret in order to execute the pick and place task. 

## SDA10F Motoman Yaskawa Setup

Setup files needed (based on the motoman package) for using the **SDA10F yaskawa robot** @ CTAI.

## Kinect

Some algorithms created using OpenCV used with OpenNI sensors. Includes the photos.cpp useful when working with a kinect sensor.

### About Centro Tecnologico De Automatizacion Industrial CTAI @ PUJ Bogotá-Colombia

The [Industrial Automation Technology Center](http://www.javeriana.edu.co/blogs/ctai/english-home/) (CTAI) is one of the finest equipped units in Latin America for teaching and research in automation and production. In the last few years, CTAI has been positioned as a leader in development and research of scientific and technological knowledge at a local and national level.

The main objective of CTAI is to contributing to the scientific and technological training of future professionals in Engineering, being a platform for research in industrial automation to help research groups as well as offering consulting services in process automation, product innovation, and machine design to further development of the local and national productive sector.

The Center boasts highly qualified human and technical resources in the product design and manufacturing of production planning and control, process simulation, computer numerical control (CNC) operations, quality control by computer vision, and computer integrated manufacturing.

*Created by Nicolas May 2018*
