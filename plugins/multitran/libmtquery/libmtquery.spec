Name: libmtquery
Version: 0.0.1
Release: alt1

Summary: multitran translation query library
License: LGPL
Group: System/Libraries

Source: %name-%{version}alpha1.tar.bz2

# Automatically added by buildreq on Tue Nov 09 2004
BuildRequires: gcc-c++ libbtree-devel libfacet-devel libmtsupport-devel libstdc++-devel

%description
multitran translation query library

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
%doc ChangeLog
%_libdir/*.so.*

%files devel
%_libdir/*.so
%_includedir/*

%files devel-static
%_libdir/*.a

%changelog
