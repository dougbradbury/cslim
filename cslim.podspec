Pod::Spec.new do |s|

  s.name         = "cslim"
  s.version      = "1.1"
  s.summary      = "An implementation of FitNesse SliM for C and Objective-C"
  s.homepage     = "https://github.com/dougbradbury/cslim"
  s.license      = { :type => 'EPL', :file => 'LICENSE' }
  s.authors      = "Robert Martin", "James Grenning", "Doug Bradbury", "Eric Myer" 
  s.source       = { :git => "https://github.com/paulstringer/cslim.git", :tag  => "v#{s.version}" }
  s.source_files = 'include/Com/*.h', 'include/CSlim/*.h', 'include/ExecutorObjectiveC/*.h', 'src/Com/*', 'src/CSlim/*', 'src/ExecutorObjectiveC/*'
  s.exclude_files = 'src/ExecutorObjectiveC/main.*'
  s.ios.source_files = 'src/ExecutorObjectiveC/main.c'
  s.ios.source_files = 'src/ExecutorObjectiveC/main.m'
  s.private_header_files = "**/*.h"
end
