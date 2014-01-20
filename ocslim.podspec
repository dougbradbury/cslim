Pod::Spec.new do |s|

  s.name         = "ocslim"
  s.version      = "1.0.0"
  s.summary      = "An implementation of FitNesse SliM for C and Objective-C"
  s.homepage     = "https://github.com/dougbradbury/cslim"
  s.license      = { :type => 'EPL', :file => 'LICENSE' }
  s.authors      = "Robert Martin", "James Grenning", "Doug Bradbury", "Eric Myer" 
  s.source       = { :git => "https://github.com/paulstringer/cslim.git", :tag  => "v#{s.version}" }
  s.source_files  = 'include/Com/*.h', 'include/CSlim/*.h', 'include/ExecutorObjectiveC/*.h', 'src/Com/*.c', 'src/Com/*.c', 'src/CSlim/.c', 'src/ExecutorObjectiveC/*.m', 'fixtures/Main.c'
  s.public_header_files = ''
  s.exclude_files = 'src/ExecutorObjectiveC/OCSReturnValue.m', 'include/ExecutorObjectiveC/OCSReturnValue.h'
  s.requires_arc = true
  
  s.subspec 'no-arc' do |sp|
  	sp.source_files = 'src/ExecutorObjectiveC/OCSReturnValue.m', 'include/ExecutorObjectiveC/OCSReturnValue.h'
  	sp.requires_arc = false
  end
end
