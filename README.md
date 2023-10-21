# Docker Deploy

> This Software is part of another and has a very specific usage only
> meant to be run for 3CS. So do not get confused with the name. 
> it does NOT deploy docker containers.  You might find some of its functions useful.

---
Run Elevated Scripts with from non-root accounts without even the non-root user having R/W/X permission on the script.
You can define 1 script file per user on the configuration. run the program on either daemon mode or using systemd.
Then user can call -a / --apply to run the script.

We use this to run/change docker-compose without the service-user having access X permission to docker/docker-compose bin.


## Documentation & Usage
- Installation
	- Built Tools & Environment
	- Dependent Packages
	- Build Instructions
	- Installation
- Usage
	- Commands
	- Server Usage
	- Client  Usage
	- Daemon
- Configuration
	- main.cfg
	- user.cfg

### Installation
####  - Built Tools & Environment
This package is built using GNU auto tools. refer to [GNU Docs](https://www.gnu.org/software/automake/manual/html_node/Autotools-Introduction.html). This program is only supported on LINUX based systems. you may use a cross-build system if you are coming from Windows/Darwin etc.

#### - Dependent Packages
Depends on Libconfig, [refer](https://hyperrealm.github.io/libconfig/libconfig_manual.html)

#### - Build Instructions
##### > Clone
Clone the repo & CD inti it.
```sh
git clone git@github.com:3CSDesign/docker-deploy.git
cd docker-deploy
```
##### > Bootstrap
if the system supports bourne shell (/bin/sh), run 
```sh
./autogen.sh
``` 
if not, manually run the equivalent commands of your build system.
```sh
aclocal
autoheader
automake --add-missing --copy
autoconf
```
##### > Configure build
Run the configure script
```sh
./configure
```
if something is missing it will fail, try again after installing / fixing the mentioned issues

##### > Build
GNU Make to build the program 
```sh
make
```
##### > Install
```sh
make install
```

Check installation by running --version
```sh
dedeploy --version
# Docker Deploy 0.0.1
```

### Usage
#### - Commands
```sh
Usage: ddeploy -c file 

User Restricted Docker Setup

  -h, --help             Print help and exit
  -V, --version          Print version and exit
  -c, --config=filename  cfg file input
  -d, --daemon           daemon  (default=off)
  -t, --test             test the cfg files (default=off)
  -a, --apply            apply - for client (default=off)
```
#### - Server Usage
By default, the program content will be installed to /etc/docker-deploy. Prepare the .cfg files (more at configuration step). Make sure the correct permissions are given. non-root user should only be able to read the files in /etc/docker-deploy (0755).  If the non-root user is given write permission, that will void the whole point of the program. 
Start the program either using systemd or just run it from the terminal using daemon mode.

##### > systemd service
For systemd, create a service file -example: `/usr/lib/systemd/system/docker-deploy.service`
```
[Unit]
Description=Super simple daemon

[Service]
Type=forking
PIDFile=/etc/docker-deploy/deploy.pid
ExecStart=/usr/local/bin/dedeploy -c /path/to/config.cfg -d 
User=root
ExecReload=/bin/kill -HUP $MAINPID

[Install]
WantedBy=multi-user.target
```

Then use systemd to start/stop/reload
```sh
systemctl start docker-deploy
systemctl status docker-deploy
systemctl reload docker-deploy
systemctl stop docker-deploy
```
##### > Other 
The Daemon option will make the program run in background `-d`|`--daemon`
```sh
dedeploy -c /path/to/config.cfg -d
```
There is no inbuilt method the kill the program. you will need to kill the PID 

#### - Client  Usage
Client must be a non-root user, Should not be able to modify the main.cfg and it's included files but read them
Simply run `-a`|`--apply` . thats all for the client
```sh
dedeploy -a
```

### Configuration
#### - main.cfg
This file is the primary configuration file which will require a few settings to be added even for the program to simply start.

##### > Required Variables
- `log_file_path` - path for logfile
- `pid_file_path` - PID file path for daemon, will fail if not set
- `deploy` - root setting for projects variable
- `deploy`>`projects` - actual list definition of the users and their files
- `deploy`>`projects`>`name` - name of the user (whoami)
- `deploy`>`projects`>`main_file` - The script that requires elevated permissions. user does not need any access to this file. not even read access. daemon will queue and execute the script when when user runs `--apply`|`-a`
- `deploy`>`projects`>`lock_file` - a file in the user directory. user need RWX permissions for this.
 ##### > Basic example
```cfg
log_file_path = "deploy.log";
pid_file_path = "deploy.pid";
// Deployment objects
deploy = {
    projects = (
      {
		    name = "test";
		    main_file = "/restricted/test/location/run.sh";
		    lock_file = "/home/test/.ddeploy.lock";
	   },
       {
		    name = "test2";
		    main_file = "/restricted/test2/location/run.sh";
		    lock_file = "/home/test2/.ddeploy.lock";
	   },
    );
}
```

 ##### > Recommended usage
```cfg
log_file_path = "deploy.log";
pid_file_path = "deploy.pid";
// Deployment objects
deploy = {
    projects = (
    @include "deploy.d/*.cfg"
    );
}
```
The `@include "deploy.d/*.cfg"` variable will tell the program to look for all files ending with .cfg inside the directory. This is very handy when using automation tools. we can use a single file per user.

Inside `deploy.d/test.cfg`,
```cfg
      {
		    name = "test";
		    main_file = "/restricted/test/location/run.sh";
		    lock_file = "/home/test/.ddeploy.lock";
	   },
```

Changes to the config file must be reloaded. for systemd, you need to run systemctl reload docker-deploy. otherwise you will need to restart the program on each config.