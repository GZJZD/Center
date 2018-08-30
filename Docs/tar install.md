# Tars开发环境配置过程
## 编译C++开发环境
### 安装依赖包
```sh
yum install flex bison -y
yum install glibc-devel -y
yum install gcc-c++ -y
yum install cmake -y
yum install git -y
```

### 源码编译mysql静态库
首先安装mysql编译过程中的依赖包
```sh
yum install ncurses-devel -y
yum install zlib-devel -y
```

获取mysql相应版本源码后解压到`/usr/local/`目录下
```sh
cd /usr/local/
tar -zxf mysql-5.6.39.tar.gz
ln -s /usr/local/mysql-5.6.39 /usr/local/mysql
```

利用`cmake`命令构建makefile后开始编译
```sh
cmake ./ -DCMAKE_INSTALL_PREFIX=/usr/local/mysql-5.6.39 -DWITH_INNOBASE_STORAGE_ENGINE=1 -DENABLE_DOWNLOADS=1 -DMYSQL_USER=mysql -DDEFAULT_CHARSET=utf8 -DDEFAULT_COLLATION=utf8_general_ci
make
make install
```

### 编译Tars框架
下载tars源码，首先进入`/Tars/cpp/thirdparty`目录,执行`thirdparty.sh`脚本，会从github下载依赖的rapidjson源码包
```sh
cd /works/Tars/cpp/build
./thirdparty.sh
```

默认的安装路径为`/usr/local/tars/cpp`，为了后期编码方便，我们将其更改到共享目录`works`中
```sh
mkdir /works/evn_tars/

#修改cpp/build目录下CMakeLists.txt文件中的安装路径
sed -i "s/\/usr\/local\/tars\/cpp/\/works\/evn_tars\/cpp/" /works/Tars/cpp/build/CMakeLists.txt

#修改cpp/servant/makefile/makefile.tars文件中的TARS_PATH的路径
sed -i "s/\/usr\/local\/tars\/cpp/\/works\/evn_tars\/cpp/" /works/Tars/cpp/servant/makefile/makefile.tars

#修改cpp/servant/script/create_tars_server.sh文件中的DEMO_PATH的路径
sed -i 's/DEMO_PATH=.*/DEMO_PATH=\/works\/evn_tars\/cpp\/script\/demo/' /works/Tars/cpp/servant/script/create_tars_server.sh

#修改cpp/servant/script/create_http_server.sh文件中的DEMO_PATH的路径
sed -i 's/DEMO_PATH=.*/DEMO_PATH=\/works\/evn_tars\/cpp\/script\/http_demo/' /works/Tars/cpp/servant/script/create_http_server.sh
```

然后进入`/Tars/cpp/build`目录，开始全量编译
```sh
cd /works/Tars/cpp/build
chmod u+x build.sh
./build.sh all
```
**编译时默认使用的mysql开发库路径：include的路径为/usr/local/mysql/include，lib的路径为/usr/local/mysql/lib/，若mysql开发库的安装路径不在默认路径，则需要修改build目录下CMakeLists.txt文件中的mysql相关的路径，再编译**

如果需要重新编译
```sh
./build.sh cleanall
./build.sh all
```

安装tars开发包
```sh
cd /works/Tars/cpp/build/
./build.sh install
```
至此，Tars的C++开发框架已经编译完成，可以直接进行服务代码的编写和编译了

## Tars框架的运行环境
### 卸载系统mariadb-lib
```sh
rpm -qa|grep mariadb
rpm -e mariadb-libs-5.5.56-2.el7.x86_64 --nodeps
yum list installed |grep mariadb
```

### 安装mysql二进制包
```
yum localinstall MySQL-server-5.6.39-1.el7.x86_64.rpm -y
yum localinstall MySQL-client-5.6.39-1.el7.x86_64.rpm -y
```

初始化数据库实例
```
[root@tars mysql]# /usr/bin/mysql_install_db --user=mysql
FATAL ERROR: please install the following Perl modules before executing /usr/bin/mysql_install_db:
```

如果出现提示缺少Data:dumper模块的错误，可通过安装`autoconf`来解决
```
yum install autoconf -y
```

设置root用户密码
```sql
mysqladmin -u root password "1"
mysqladmin -u root -p oldpass password "newpass"(已有密码修改)
```

添加用户授权
```sql
mysql -u root -p
use mysql;

# 为root用户远程访问授权
grant all on *.* to 'root'@'%' identified by '123' with grant option;
grant all on *.* to 'root'@'localhost' identified by '123' with grant option;
grant all on *.* to 'root'@'127.0.0.1' identified by '123' with grant option;

# 为tars用户添加本地和远程访问授权
grant all on *.* to 'tars'@'%' identified by 'tars2015' with grant option;
grant all on *.* to 'tars'@'localhost' identified by 'tars2015' with grant option;
grant all on *.* to 'tars'@'tars.gzjg.com' identified by 'tars2015' with grant option;
grant all on *.* to 'tars'@'192.168.3.189' identified by 'tars2015' with grant option;

# 查询当前用户授权信息
select host,user,password from user;
```

mysql的相关命令
```
chkconfig mysql on
service mysql stop
service mysql start
```

### 创建数据库
sql脚本在cpp/framework/sql目录下，修改部署的ip信息
```sh
cd /works/Tars/cpp/framework/sql/
db_ip=192.168.3.101
host_ip=192.168.3.189
sed -i "s/192.168.2.131/${host_ip}/g" `grep 192.168.2.131 -rl ./*`
sed -i "s/db.tars.com/${db_ip}/g" `grep db.tars.com -rl ./*`
```
**注意，192.168.2.131这个ip是tars开发团队当时部署服务测试的ip信息，替换成自己数据库的部署地址即可,不要是127.0.0.1**
**注意，db.tars.com是tars框架数据库部署的地址信息，替换成自己数据库的部署地址即可**

修改脚本中的root用户密码信息
```sh
sed -i "s/root@appinside/1/g" `grep root@appinside -rl ./exec-sql.sh`
```

执行数据库导入脚本
```sh
./exec-sql.sh
```
脚本执行后，会创建3个数据库，分别是db_tars、tars_stat、tars_property。
其中db_tars是框架运行依赖的核心数据库，里面包括了服务部署信息、服务模版信息、服务配置信息等等；
tars_stat是服务监控数据存储的数据库；
tars_property是服务属性监控数据存储的数据库；

### java jdk环境
下载java jdk，解压安装
```sh
cd /usr/local
tar -zxf jdk-8u112-linux-x64.tar.gz
ln -s /usr/local/jdk1.8.0_112 /usr/local/java
```

### 设置JAVA_HOME
```sh
java_var=$(sed -n '/^#JAVA_HOME$/'p /etc/profile)
if [ ! $java_var ]; then
  echo "JAVA_HOME UNSET"
cat << 'eof' >> /etc/profile
#JAVA_HOME
export JAVA_HOME=/usr/local/java
export JRE_HOME=$JAVA_HOME/jre

export CLASSPATH=.:$JAVA_HOME/lib:$CLASSPATH
export PATH=$JAVA_HOME/bin:$PATH

eof
source /etc/profile
else
  echo "JAVA_HOME EXIST"
fi
```

### maven安装
```sh
yum install maven -y
mvn -v
```

修改maven安装目录下的conf目录的setting.xml，添加阿里的maven镜像
```sh
sudo vim settings.xml
...
<mirror>
    <id>nexus-aliyun</id>
    <mirrorOf>central</mirrorOf>
    <name>Nexus aliyun</name>
    <url>http://maven.aliyun.com/nexus/content/groups/public</url>
</mirror>
...
```

### mvn生成本地依赖包
```sh
mvn clean install
mvn clean install -f core/client.pom.xml
mvn clean install -f core/server.pom.xml
```
此时会本地仓库中生成`tars-client-1.5.0`和`tars-server-1.5.0`两个依赖包

### 修改web中数据库配置
修改配置文件，文件存放的路径在web/src/main/resources/目录下
```sh
cd /works/Tars/web/src/main/resources
db_ip=192.168.3.101
host_ip=192.168.3.189
#修改为实际的数据库配置
sed -i "s/tarsweb.datasource.tars.addr=.*/tarsweb.datasource.tars.addr=${db_ip}/" app.config.properties

#替换registry1.tars.com，registry2.tars.com为实际IP。可以只配置一个地址，多个地址用冒号“:”连接
sed -i "s/locator =.*/locator = tars.tarsregistry.QueryObj@tcp -h ${host_ip} -p 17890/" tars.conf
```

### 打包tars.war
```sh
cd /works/Tars/web
mvn clean package
```
在web目录下执行命令，会在web/target目录下生成tars.war

### 安装web容器resin
cd /usr/local/
tar -zxf resin-4.0.55.tar.gz
ln -s /usr/local/resin-4.0.55 /usr/local/resin

### web服务发布和启动
将tars.war放置到/usr/local/resin/webapps/中，然后创建日志目录
```sh
#拷贝war包
cp /works/Tars/web/target/tars.war /usr/local/resin/webapps/

#日志目录
mkdir -p /data/log/tars
```

修改Resin安装目录下的`conf/resin.xml`配置文件
```xml
<host id="" root-directory=".">
    <web-app id="/" root-directory="webapps/ROOT"/>
</host>s

```
修改为
```xml
<host id="" root-directory=".">
    <web-app id="/" document-directory="webapps/tars"/>
</host>
```

启动resin
```sh
/usr/local/resin/bin/resin.sh start
```

访问站点
浏览器输入`http://192.168.3.191:8080`，即可web管理页面

## 框架服务安装
### 核心服务
在开发环境机器上生成framework.tgz核心包
```sh
cd /works/Tars/cpp/build/
make framework-tar
```

创建框架服务目录
```sh
mkdir -p /usr/local/app/tars
```

拷贝framework.tgz并解压
```sh
cp /works/Tars/cpp/build/framework.tgz /usr/local/app/tars
cd /usr/local/app/tars
tar xzfv framework.tgz
```

替换框架服务conf文件的ip地址配置
```sh
db_ip=192.168.3.101
host_ip=192.168.3.189

sed -i "s/192.168.2.131/${host_ip}/g" `grep 192.168.2.131 -rl ./*`
sed -i "s/db.tars.com/${db_ip}/g" `grep db.tars.com -rl ./*`
sed -i "s/registry.tars.com/${host_ip}/g" `grep registry.tars.com -rl ./*`
sed -i "s/web.tars.com/${host_ip}/g" `grep web.tars.com -rl ./*`
```
**注意，192.168.2.131这个ip是tars开发团队当时部署服务测试的ip信息，替换成本机的部署地址即可，不要是127.0.0.1**

**注意，db.tars.com是tars框架数据库部署的地址信息，替换成自己数据库的部署地址即可**

**注意，registry.tars.com是tars框架主控tarsregistry服务部署的地址信息，替换成自己主控tarsregistry符的部署地址即可**

**注意，web.tars.com是rsync使用的地址信息，替换成自己的部署机器地址即可**

执行tars_install.sh脚本安装核心服务
```sh
./tars_install.sh
tarspatch/util/init.sh
```

```sh
tarsregistry/util/stop.sh
tarsAdminRegistry/util/stop.sh;
tarsnode/util/stop.sh ;
tarsconfig/util/stop.sh;
tarspatch/util/stop.sh;
```

重启WEB服务
```sh
/usr/local/resin/bin/resin.sh restart
```

### 其他服务
分别编译以下服务的安装包
```sh
make tarsstat-tar
make tarsnotify-tar
make tarsproperty-tar
make tarslog-tar
make tarsquerystat-tar
make tarsqueryproperty-tar
```

### 手动部署其他服务
部署`tarsnotify`
> 在安装核心基础服务时，默认`tarsnotify`服务的部署信息已初始化了，安装完管理平台后，就可以看到。我们只需要完成发布操作就可以了。

部署`tarsstat`
首先在运维管理->服务部署页面来完成部署申请，补充服务的以下部署信息。
```sh
应用: tars
名称: tarsstat
类型: tars_cpp
模板: tars.tarsstat
OBJ名: StatObj
端口: 10003
```

部署`tarsproperty`
首先在运维管理->服务部署页面来完成部署申请，补充服务的部署信息，也就是完成一个服务占位。
```sh
应用: tars
名称: tarsproperty
类型: tars_cpp
模板: tars.tarsproperty
OBJ名: PropertyObj
端口: 10004
```

部署`tarslog`
首先在运维管理->服务部署页面来完成部署申请，补充服务的部署信息，也就是完成一个服务占位。
```sh
应用: tars
名称: tarslog
类型: tars_cpp
模板: tars.tarslog
OBJ名: LogObj
端口: 10005
```

部署`tarsquerystat`
首先在运维管理->服务部署页面来完成部署申请，补充服务的部署信息，也就是完成一个服务占位。
```sh
应用: tars
名称: tarsquerystat
类型: tars_cpp
模板: tars.tarsquerystat
OBJ名: NoTarsObj
端口: 10006
协议: 非TARS
```

部署`tarsqueryproperty`
首先在运维管理->服务部署页面来完成部署申请，补充服务的部署信息，也就是完成一个服务占位。
```sh
应用: tars
名称: tarsqueryproperty
类型: tars_cpp
模板: tars.tarsqueryproperty
OBJ名: NoTarsObj
端口: 10007
协议: 非TARS
```

## 开发交易平台
```sh
TraderCenter
TraderServer
Test1
Test2

/works/evn_tars/cpp/script/create_tars_server.sh TraderCenter TraderServer EFServant

/works/evn_tars/cpp/script/create_tars_server.sh Center EFServer TraderServant
```
