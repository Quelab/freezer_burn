# == Class: freezer_burn
#
# Full description of class freezer_burn here.
#
# === Parameters
#
# Document parameters here.
#
# [*sample_parameter*]
#   Explanation of what this parameter affects and what it defaults to.
#   e.g. "Specify one or more upstream ntp servers as an array."
#
# === Variables
#
# Here you should define a list of variables that this module would require.
#
# [*sample_variable*]
#   Explanation of how this variable affects the function of this class and if
#   it has a default. e.g. "The parameter enc_ntp_servers must be set by the
#   External Node Classifier as a comma separated list of hostnames." (Note,
#   global variables should be avoided in favor of class parameters as
#   of Puppet 2.6.)
#
# === Examples
#
#  class { 'freezer_burn':
#    servers => [ 'pool.ntp.org', 'ntp.local.company.com' ],
#  }
#
# === Authors
#
# Author Name <author@domain.com>
#
# === Copyright
#
# Copyright 2015 Your name here, unless otherwise noted.
#
class freezer_burn {
  include 'nodejs'
  package {'npm': ensure => present}
  package {'git': ensure => present}
  package {'libusb-1.0-0-dev': ensure => present}

  file {'/usr/bin/node':
  }
  package { 'green-bean':
    ensure   => present,
    provider => 'npm',
    require => Package['git', 'npm', 'libusb-1.0-0-dev']
  }

  package { 'firebase':
    ensure  => present,
    provider => 'npm',
    require =>  Package['npm']
  }

}
