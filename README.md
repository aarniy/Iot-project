# Iot-project

Main source:
https://labs.iot-lab.info/ 
This project has 2 phase:
1-	Sensing Layer:
Using https://www.iot-lab.info/ Fit IoT Testbed and https://www.riot-os.org/ Rıot Os, Sensor data(temperature and humidity) collecting from simulation.
2-	Cloud layer:  Using Amazon Web Services AWS IoT https://aws.amazon.com/iot/ Amazon Web to transmit the measurements to the cloud.

1.	Sensing Layer:
In this project, we use The B-L072Z-LRWAN1 board(https://www.st.com/en/evaluation-tools/b-l072z-lrwan1.html) is LoRa capable board running on ab ARM CortexM0+ STM32L072CZ microcontroller. We connect HTS221 temperature and humidity sensor to this board in the simulation. Aim is to tranmit simulation made temperature and humidity data to the cloud. 
2.	Cloud layer:  
In this project we connected to simulation environment to AWS IoT Core.  We created a thing in our AWS account for IoT Core and obtain security credentials and other connection settings. Incorporate code into our board using MQTT or Iot Analytics database. Than we send Data to AWS IoT Core. Than we connected our LoRaWAN Devices from The Things Stack to Iot Analytics database. LoRaWAN is a technology built upon Low Power Wide Area Network (LPWAN).  We use Iot Analytics database to store, process, or integrate data.


Starting Point:

First you need make AWS IoT configuration.
For The Things Network configuration, you need to open an account in this site https://www.thethingsnetwork.org/
https://www.thethingsindustries.com/docs/integrations/cloud-integrations/aws-iot/

Than you have to create and application on this platform. Application ID dependable for user. Then you need to register a device. After you generate a device EUI, register it. In order to effectively handle the messages originating from the RIOT application, go to Payload Formats and insert the function we use into the decoder input field.


Than you need to connect RIOT Application:
https://doc.riot-os.org/index.html 
Preliminary requirements
(You must have installed:
Git. 
The build-essential packet (make, gcc etc.). This varies based on the operating system in use.
The Native dependencies. 
OpenOCD.
GCC Arm Embedded Toolchain. 
Netcat with IPv6 support.)

For the installation command is the following:
> sudo apt-get install netcat-openbsd
Application set-up:
	For creating the project, you need to clone the RIOT repository:
> git clone git://github.com/RIOT-OS/RIOT.git
In order to publish, messages containing the virtual sensors’ values on a TTN device, you must update (RIOT/examples/lorawan/main.c) and (RIOT/examples/lorawan/Makefile)
Than open the Terminal from directory lorawan and run the following command:
> make all
Now you can go on by starting a new experiment on the IoT-LAB testbed platform.

IoT-LAB:
When you open the IoT-LAB account, set up your ssh access to the IoT-LAB servers. After you took it you can start the set-up an experiment set-up. After pushing new experiment, Fill the form with name and duration of your choice.  Than you need to choose a node and architecture. After your selection, add this experiment. Press the firmware icon. Choose the.elf file you created earlier by clicking on Browse. Than write (ssh <login>@grenoble.iot-lab.info) to connect to the IOT-LAB server execution. Now, use Netcat to communicate with the st-lrwan1 node, substituting your node ID to <nodeID> (> nc st-lrwan1-<nodeID> 20000). 

TTN-AWS Integration:
After you take AWS EC2 key pair. Log in AWS CloudFormation Console. Find Stacks and create a stack. Enter template is ready and amazon S3 URL. Enter the stack name, ID, and access key from your TTN application in the form. Complete the AWS IoT and Elastic Beanstalk settings sections. After a couple of mins, stacks should be available.


The project involves routing data from AWS IoT Core to IoT Analytics, performing SQL queries on the data, and visualizing it using Amazon QuickSight.
Prerequisites:
Before starting the configuration process, ensure that you have the following:
HTS221 temperature and humidity sensor.
Knowledge of AWS IoT Core, IoT Analytics, and Amazon QuickSight.

Connect IoT Core to IoT Analytics:
Create a Rule in AWS IoT Core:
 In AWS IoT Core, create a rule to forward data from your device to IoT Analytics.
 Specify an action to send the data to IoT Analytics.

Set Up IoT Analytics Pipeline
Create a Channel:
   In IoT Analytics, create a channel to ingest data from IoT Core.
   Configure the channel to receive data from the rule you created.
Create a Pipeline:
  Set up a pipeline in IoT Analytics to process and store the data.
   Define activities, such as filtering or transforming the data, within the pipeline.

Define IoT Analytics Data Set
Create a Data Set:
   Define a data set in IoT Analytics that corresponds to your sensor data.
   Specify the schema and data storage options for the data set.

Run SQL Queries:
https://learn.microsoft.com/en-us/sql/ssms/visual-db-tools/run-queries-visual-database-tools?view=sql-server-ver16
   Use the SQL capabilities in IoT Analytics to query and manipulate the data as needed.
   Apply SQL queries to clean, filter, or aggregate the data.

Visualize Data with QuickSight 
https://docs.aws.amazon.com/quicksight/latest/user/working-with-visual-types.html
Connect QuickSight to IoT Analytics:
   Set up an Amazon QuickSight account and connect it to the IoT Analytics data set.
   Define the data source in QuickSight.

Create Visualizations:
   - Build visualizations and dashboards in QuickSight using the processed data.
   - Explore different visualization options based on your project requirements.

Error Handling and Optimization
 Implement error handling mechanisms in IoT Core rules and IoT Analytics pipelines.
 Optimize IoT Analytics queries for performance.
 Monitor and troubleshoot any issues in the data flow.


Additional sources: 
https://www.thethingsindustries.com/docs/integrations/adding-applications/
https://www.thethingsindustries.com/docs/integrations/cloud-integrations/aws-iot/
https://doc.riot-os.org/index.html
https://aws.amazon.com/blogs/apn/connecting-your-lorawan-devices-from-the-things-stack-to-aws-iot-core/
https://labs.iot-lab.info
https://www.hackster.io/lorella-landi/riot-os-application-connected-to-aws-iot-through-ttn-157726#code
https://www.iot-lab.info/docs/boards/st-b-l072z-lrwan1/
