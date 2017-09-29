use warnings; use strict;
use Cwd; use autodie;

my $original_dir = getcwd();

mkdir("antlr");
chdir("antlr");
`wget http://www.antlr3.org/download/antlr-3.5.2-complete.jar > antlr_jar_wget_stdout.txt 2> antlr_jar_wget_stderr.txt`;
`wget http://www.antlr3.org/download/C/libantlr3c-3.4.tar.gz  > libantlr_wget_stdout.txt 2> libantlr_wget_stderr.txt`;
`tar -xvzf libantlr3c-3.4.tar.gz > libantlr_tar_stdout.txt 2> libantlr_tar_stderr.txt`;
chdir("libantlr3c-3.4");
`./configure --enable-64bit --prefix=$original_dir/antlr/install > libantlr_configure_stdout.txt 2> libantlr_configure_stderr.txt`;
`make > make_stdout.txt 2> make_stderr.txt`;
`make install > install_stdout.txt 2> install_stderr.txt`;
chdir($original_dir);

mkdir("boost");
chdir("boost");
`wget http://sourceforge.net/projects/boost/files/boost/1.60.0/boost_1_60_0.tar.gz > boost_wget_stdout.txt 2> boost_wget_stderr.txt`;
`tar -xvzf boost_1_60_0.tar.gz > boost_tar_stdout.txt 2> boost_tar_stderr.txt`;
chdir("boost_1_60_0");
`./bootstrap.sh --prefix=$original_dir/boost/install > bootstrap_stdout.txt 2> bootstrap_stderr.txt`;
`./b2 install > b2_stdout.txt 2> b2_stderr.txt`;
chdir($original_dir);

=pod
This script downloads and installs the versions of ANTLR and BOOST required to build FullMonte
=cut
