# -*- mode: ruby -*-
# vi: set ft=ruby :

# All Vagrant configuration is done below. The "2" in Vagrant.configure
# configures the configuration version (we support older styles for
# backwards compatibility). Please don't change it unless you know what
# you're doing.
Vagrant.configure(2) do |config|
  config.vm.box = "puppetlabs/ubuntu-14.04-64-puppet"
  config.vm.network "public_network"
  # Create a forwarded port mapping which allows access to a specific port
  # within the machine from a port on the host machine. In the example below,
  # accessing "localhost:8080" will access port 80 on the guest machine.
  # config.vm.network "forwarded_port", guest: 80, host: 8080

  # Create a private network, which allows host-only access to the machine
  # using a specific IP.
  # config.vm.network "private_network", ip: "192.168.33.10"

  config.vm.synced_folder "puppet/hieradata", "/var/lib/hieradata"

  config.vm.provider "vmware_fusion" do |vb|  
    # Customize the amount of memory on the VM:
    vb.memory = "1024"
  end
  config.vm.provision "puppet" do |puppet|
    puppet.hiera_config_path = "hiera.yaml"
    puppet.module_path =       'puppet/modules'
    puppet.options =           "--verbose --debug"
  end
end
