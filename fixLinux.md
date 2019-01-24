# Fix My Linux

## 问题1

### 问题描述

安装软件过程中报错：`E: Sub-process /usr/bin/dpkg returned an error code (1)`

### 解决方法

可写成`dpkg.sh`脚本自动执行

```shell
sudo mv /var/lib/dpkg/info /var/lib/dpkg/info.bak
sudo mkdir /var/lib/dpkg/info
sudo apt-get update
# appname 替换为报错的 程序名称
sudo apt-get -f install app-name
sudo mv /var/lib/dpkg/info/* /var/lib/dpkg/info.bak
sudo rm -rf /var/lib/dpkg/info
sudo mv /var/lib/dpkg/info.bak /var/lib/dpkg/info
```

#### 可能导致的问题

出现大量的报错：`dpkg: warning: files list file for package 'XXXX' missing; assuming package has no files currently installed`

解决办法：

将报错的打印信息复制到txt文件中，执行python程序生成`fix.sh`脚本

```python
#!/usr/bin/env python
# -*- coding: utf-8 -*-

import re
def main():
    fix = open('fix.sh', 'w+')
    # 1.txt为打印信息的文本
    for line in open("1.txt"):
        pkg = re.match(re.compile('''dpkg: warning: files list file for package '(.+)' '''), line)
        if pkg:
            cmd = "sudo apt-get install --reinstall -y " + pkg.group(1)
            fix.write(cmd + '\n')
if __name__ == "__main__":
    main()
```

`fix.sh`脚本中的内容为：`sudo apt-get install --reinstall -y XXXXXX`

该脚本执行时间较长，如果仍出现问题`E: Sub-process /usr/bin/dpkg returned an error code (1)`，暂停程序重复执行脚本`dpkg.sh`，并重新复制内容到1.txt中循环往复

## 问题2

### 问题描述

ubuntu 18.04 如何正确的安装mysql 5.7？
出现的问题有：

- 安装过程中未出现提示输入root用户的默认密码，彻底卸载重装仍未出现提示
- 输入`mysql -u root -p`报错如：
  - `ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/run/mysqld/mysqld.sock' (2)`
  - `ERROR 1045 (28000): Access denied for user 'root'@'localhost' (using password: YES)`

#### 解决办法

**官方！官方！官方！**

如果安装了mysql请卸载： `sudo apt-get purge mysql-common mysql-server mysql-client`

去[官方网站](https://www.mysql.com/downloads/)下载系统对应版本的安装程序

或[下载](https://dev.mysql.com/downloads/repo/apt/)mysql-apt-config_0.8.10-1_all.deb配置程序，点击安装，选择要安装的mysql版本，然后参考[官方文档](https://dev.mysql.com/doc/mysql-apt-repo-quick-guide/en/)继续安装

## Linux 美化

安装插件

- Chrome/Firefox 安装gnome扩展插件
- 访问gnome extensions 网站添加插件
- Alternatetab
- Dynamic top bar
- No title bar
- OpenWeather
- Screenshot tool
- Screenshot window sizer
- User themes