require 'spec_helper'
describe 'freezer_burn' do

  context 'with defaults for all parameters' do
    it { should contain_class('freezer_burn') }
  end
end
