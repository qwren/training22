SUMMARY = "helloworld of makefile recipe"
DESCRIPTION = "My hello world application"
LICENSE = "CLOSED"

FILESEXTRAPATHS_prepend := "${THISDIR}/:"
SRC_URI = "file://helloworld/"

S = "${WORKDIR}/helloworld"

CFLAGS_append = "-Wall -O -g"

EXTRA_OEMAKE = "'CC=${CC}' 'CFLAGS=${CFLAGS}'"
EXTRA_OEMAKE_append = " 'LDFLAGS=${LDFLAGS}'"
EXTRA_OEMAKE_append = " 'DESTDIR=${D}'"

do_install () {
        oe_runmake install
}

# if no install target in makefile, using this do_install
#do_install () {
#install -d ${D}${bindir}/
#install -m 0755 ${S}/helloworld ${D}${bindir}/
#}

# if no LDFLAGS, using this
#INSANE_SKIP_${PN} += "ldflags"

FILES_${PN} = "${bindir}/"

