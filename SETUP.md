前言：如果你不喜欢折腾，那么请直接按 [yyhhenry](https://github.com/yyhhenry/oslab) 的方法安装 Ubuntu 20.04 并配置 WSL2。

如果你并不喜欢 Ubuntu 20.04，或者讨厌往自己的 WSL2 里安装奇奇怪怪的老版本库和过时的编译器，可以按下面的方法通过容器安装 Ubuntu 20.04 并配置 X11 Forwarding：

1. 使用 `uname -a` 查看内核版本，去 [WSL2-Linux-Kernel](https://github.com/microsoft/WSL2-Linux-Kernel) 的 Release 下载对应版本的内核并按指示重新编译 WSL2 的 Linux 内核。确保 `make menuconfig` 的时候勾选 `Miscellaneous filesystems` 中的 `Minix file-system support`。

2. 修改 `/etc/wsl.conf`，加入以下代码：
```bash
[boot]
systemd=true
```

3. 在 cmd 或 Powershell 中关闭 WSL2：`wsl --shutdown`，然后重新进入，安装 Docker：`sudo apt update && sudo apt install docker`，然后拉取镜像： `sudo docker pull ubuntu:20.04`

4. `sudo docker run -t -d -p 2222:22 --privileged --volume="/tmp/.X11-unix:/tmp/.X11-unix" --volume="/root/oslab:/root/workspace" --name oslab ubuntu:20.04 bash`，`2222` 可以改成自己想要的 ssh 端口号，`/root/oslab` 为指定将宿主机哪个目录挂载进容器

5. 使用 `sudo docker exec -it oslab bash` 进入容器执行 `apt update && apt install vim` 然后 `vim /etc/apt/sources.list` 参照[清华镜像源](https://mirrors.tuna.tsinghua.edu.cn/help/ubuntu/)换源，然后 `apt install openssh-server` 安装 ssh 服务器。

6. 在容器内 `vim /etc/ssh/sshd_config` 确保有 `X11Forwarding yes`，并执行 `service ssh restart`。

7. 按实验环境配置说明来配置环境。可参考 [yyhhenry](https://github.com/yyhhenry/oslab) 的补充。

8. 此后既可以通过 `sudo docker exec -it oslab bash` 进入容器操作，也可以通过 `ssh -X root@172.17.0.2` 进入容器，容器内的窗口会被转发出来在 Windows 上显示。