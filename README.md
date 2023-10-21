# Docker Deploy

This Software is part of another and has a very specific usage only meant to be run for 3CS. You might find some of its functions usefull.

---
Run Elevated Scripts with from non-root accounts without even the non-root user having R/W/X permission on the script.
You can define 1 script file per user on the configuration. run the program on either daemon mode or using systemd.
Then user can call -a / --apply to run the script.

We use this to run/change docker-compose without the service-user having access X permission to docker/docker-compose bin.


## Installation
- 