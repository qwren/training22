### yocto

1. environment setup
$ mkdir yocto
$ git clone git://git.yoctoproject.org/poky
$ cd poky
$ git branch -a
$ git checkout -t origin/hardknott -b my-poky
$ cd ..
$ source oe-init-build-env
$ cd build
$ bitbake core-image-minimal
$ runqemu qemux86-64
$ bitbake-layers show-recipes
$ bitbake -e linux-yocto | grep ^S=

$ bitbake core-image-minimal –c populate_sdk
$ ./tmp/deploy/sdk/poky-glibc-x86_64-core-image-minimal-core2-64-qemux86-64-toolchain-3.1.2.sh 

2. add opensourced layer
$ cd poky
$ git clone https://github.com/intel/meta-intel-sgx.git
$ cd ../build
$ source oe-init-build-env
$ bitbake-layers add-layer ../poky/meta-intel-sgx
$ bitbake-layers remove-layer ../poky/meta-intel-sgx
$ bitbake-layers show-layers

$ bitbake-layers layerindex-fetch -b master meta-intel-sgx

3. create a layer
$ bitbake-layers create-layer ../poky/meta-mylayer
$ bitbake-layers add-layer ../poky/meta-mylayer
$ bitbake example
$ ls tmp/work/core2-64-poky-linux/example/0.1-r0/

4. helloworld recipes
#e.g. poky/meta-skeleton/recipes-kernel/hello-mod/files
$ cd poky/meta-mylayer
$ mkdir recipes-helloworld
$ cd recipes-helloworld

* based on makefile
$ mkdir helloworld-m/helloworld
$ cd hellowolrd-m/helloworld
$ vim helloworld.c
#‘_’是用于分隔recipe名和版本号得，不能出现helloworld_m.bb这样的名字
$ vim helloworld-m.bb
$ bitbake helloworld-m
$ file tmp/work/core2-64-poky-linux/helloworld-m/1.0-r0/image/usr/bin/helloworld
$ ls tmp/work/qemux86_64-poky-linux/core-image-minimal/1.0-r0/rootfs/usr/bin/helloworld*
$ ls tmp/work/core2-64-poky-linux/helloworld-a/1.0-r0/recipe-sysroot/usr/lib/
$ ls tmp/work/core2-64-poky-linux/helloworld-a/1.0-r0/recipe-sysroot-native/usr/lib/
#need to bridge recipe and target
$ vim conf/local.conf #第一种临时的方法
IMAGE_INSTALL += "helloworld-m"
IMAGE_INSTALL += "helloworld-a"
IMAGE_INSTALL += "helloworld-c"
$ cd meta-mylayer #第二种方法添加目标的bbappend文件
$ mkdir -p recipes-minimal/images
$ vim recipes-minimal/images/core-image-minimal.bbappend
IMAGE_INSTALL += "helloworld-m"
IMAGE_INSTALL += "helloworld-a"
IMAGE_INSTALL += "helloworld-c"
$ source oe-init-build-env
$ bitbake core-image-minimal
$ runqemu qemux86-64
$ ls /usr/bin/helloworld*

* based on autotools 

* based on cmake

* only source files

5. devtool
$ devtool add [--srcbranch <branchname>] <recipe> <uri>
$ devtool add [--srcrev <tag>] <recipe> <uri>
$ devtool add [--srcrev <commit id>] <recipe> <uri>
$ devtool build <recipe>
$ devtool build-image core-image-minimal
$ devtool deploy-target <recipe> <target>:
$ devtool -a undeploy-target <recipe> <target>:
$ runqemu qemux86-64
$ devtool finish <recipe> meta-mylayer
# see above, install the app to rootfs
$ vim tmp/deploy/images/qemux86-64/core-image-minimal-qemux86-64.manifest
$ devtool modify <recipe>

$ devtool add test https://github.com/qwren/test.git
$ devtool edit-recipe test
$ devtool build test
$ devtool finish test meta-mylayer
$ devtool modify alsa-utils
$ vim workspace/appends/alsa-utils_1.2.4.bbappend
$ devtool finish alsa-utils meta-mylayer

$ devtool create-workspace [path/to/workspace]
$ devtool status
$ devtool search <recipe>

6. hello kernel module
$ cd poky
$ mkdir -p meta-mylayer/recipes-mymodules
$ cp -r meta-skeleton/recipes-kernel/hello-mod/ meta-mylayer/recipes-mymodules/
$ cd build
$ bitbake hello-mod
$ ls tmp/work/qemux86_64-poky-linux/hello-mod/0.1-r0/image/
#need to bridge recipe and target
$ cd poky
$ vim meta/conf/machine/qemux86-64.conf
+MACHINE_EXTRA_RRECOMMENDS = "kernel-module-snd-ens1370 kernel-module-snd-rawmidi kernel-module-hello"
$ cd build
$ bitbake hello-mod
$ bitbake core-image-minimal
$ ls tmp/work/qemux86_64-poky-linux/core-image-minimal/1.0-r0/rootfs/lib/modules/5.10.107-yocto-standard/extra


### 


