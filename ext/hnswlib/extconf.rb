# frozen_string_literal: true

require 'mkmf'

$CXXFLAGS << ' -std=c++14'
$INCFLAGS << " -I$(srcdir)/src"
$VPATH << "$(srcdir)/src"

create_makefile('hnswlib/hnswlibext')
