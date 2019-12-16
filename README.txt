//this program are supported on linux-OS  
//you shold excute following command on directory that does include this repos's files


//1.install library
sudo apt-get install -y mpg321 
sudo apt-get install libncurses5-dev libncursesw5-dev


//2.complie by makefile
make


//3.search keyboard event handler number
cat /proc/bus/input/devices


//4.excute game
sudo ./rhythm /dev/input/eventX //(X = event handler number) 

//but music files, note files be prepared
//as "Prototype.mp3", "sample.txt"

key= 's', 'd', 'f', ' ', 'j', 'k', 'l'.
