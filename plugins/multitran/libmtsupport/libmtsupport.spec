Name: libmtsupport
Version: 0.0.1
Release: alt1

Summary: general support library for multitran
License: LGPL
Group: System/Libraries

Source: %name-%{version}alpha1.tar.bz2

# Automatically added by buildreq on Mon Oct 04 2004
BuildRequires: gcc-c++ libstdc++-devel

%description
Library with some usefull functions used in multitran

%package devel
Summary: Development part of %name
Group: Development/C++
Requires: %name = %version-%release

%description devel
Contents header files and development libraries for %name

%package devel-static
Summary: Static libraries for %name
Group: Development/C++
Requires: %name-devel = %version-%release

%description devel-static
Contents static libraries for %name

%prep
%setup -q -n %name-%{version}alpha1

%build
%make_build

%install
%makeinstall DESTDIR=$RPM_BUILD_ROOT

%post -p %post_ldconfig
%postun -p %postun_ldconfig

%files
#_doc ChangeLog
%_libdir/*.so.*

%files devel
%_libdir/*.so
%_includedir/*

%files devel-static
%_libdir/*.a

%changelog
