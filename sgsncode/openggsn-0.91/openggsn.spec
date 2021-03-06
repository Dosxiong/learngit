Summary:   Open Source Gateway GPRS Support Node (GGSN)
Name:      openggsn
Version:   0.91
Release:   1
URL:       http://sourceforge.net/projects/ggsn/
Source0:   http://prdownloads.sourceforge.net/ggsn/%{name}-%{version}.tar.gz
License:   GPL
Group:     System Environment/Daemons
BuildRoot: %{_tmppath}/%{name}-root

%description 
OpenGGSN is a Gateway GPRS Support Node (GGSN). It is used by mobile
operators as the interface between the Internet and the rest of the
mobile network infrastructure. The project also provides an SGSN
emulator suitable for GPRS core network testing.

%prep
%setup -q

%build

./configure --prefix=/usr --enable-static-exec

make

%install

make install prefix=$RPM_BUILD_ROOT/usr
strip $RPM_BUILD_ROOT/usr/bin/ggsn
strip $RPM_BUILD_ROOT/usr/bin/sgsnemu

#Copy ggsn init script in place
mkdir -p $RPM_BUILD_ROOT/etc/rc.d/init.d
install -m755 examples/ggsn.init \
	$RPM_BUILD_ROOT/etc/rc.d/init.d/ggsn

#Copy ggsn.conf in place
install -m755 examples/ggsn.conf \
	$RPM_BUILD_ROOT/etc/ggsn.conf

#Copy gsn_restart file in place
mkdir -p $RPM_BUILD_ROOT/var/lib/ggsn
echo "0" > $RPM_BUILD_ROOT/var/lib/ggsn/gsn_restart

#Clean up unwanted library files
rm -rf $RPM_BUILD_ROOT/usr/include/*
rm -rf $RPM_BUILD_ROOT/usr/lib/*


%clean
rm -rf $RPM_BUILD_ROOT
make clean

%post
/sbin/chkconfig --add ggsn

%files
%defattr(-,root,root)

/usr/bin/ggsn
/usr/bin/sgsnemu
/etc/rc.d/init.d/ggsn
%dir /var/lib/ggsn
/var/lib/ggsn/gsn_restart

%doc AUTHORS ChangeLog COPYING INSTALL NEWS README 
%doc examples/ggsn.conf
%doc examples/sgsnemu.conf
%doc examples/ggsn.init
%doc examples/firewall
%doc /usr/man/man8/ggsn.8.gz
%doc /usr/man/man8/sgsnemu.8.gz

%config /etc/ggsn.conf


#/usr/lib/libgtp.a
#/usr/lib/libgtp.la
#/usr/lib/libgtp.so
#/usr/lib/libgtp.so.0
#/usr/lib/libgtp.so.0.0.0



%changelog
* Mon Jun 30 2003  <jj@openggsn.org>
- Initial build.
