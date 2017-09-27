#!/usr/bin/perl
use strict; use warnings;
use Cwd; use autodie; use POSIX qw(uname);
#use Data::Dump qw(dump);
#use GetOpt::Long;

#GLOBAL VARIABLES AND PROGRAM
my $original_dir = getcwd();
my $git_repo_dir = "$original_dir/fullmonte_source_repositories";
&main;
exit;

#FUNCTIONS
sub HEADER {
    # print a HEADER message
    # e.g. ($length, $symbol, $message) = (12, =, TEST)
    #
    # ============      $top_bottom_boundary
    # === TEST ===      $side_boundary + $message + $side_boundary
    # ============

    (my $message) = @_;
    my $length = 80;
    my $symbol = "=";
    my $top_bottom_boundary = $symbol x $length;
    my $side_boundary = $symbol x int(($length - length($message))/2);

    print "$top_bottom_boundary\n";
    print "$side_boundary";
    print "$message";
    print "$side_boundary\n";
    print "$top_bottom_boundary\n";

    return;
}

sub mcd {
    # make and change directory
    (my $directory) = @_;
    mkdir($directory);
    chdir($directory);
    return;
}

sub clone_git_repositories {
    mkdir($git_repo_dir);
    
    my $betzgrp_repository_server = "pcjeff";
    print "Ensure you have $betzgrp_repository_server in your .ssh/config file. You should be able to login with 'ssh $betzgrp_repository_server'\n";

    my @git_clone_commands;
    #push @git_clone_commands, "git clone https://github.com/ibm-capi/pslse $git_repo_dir/pslse";
    #push @git_clone_commands, "git clone https://github.com/ibm-capi/libcxl $git_repo_dir/libcxl";
    #push @git_clone_commands, "git clone $betzgrp_repository_server:/home/jcassidy/git-repos/BlueCMake.git $git_repo_dir/BlueCMake";
    #push @git_clone_commands, "git clone $betzgrp_repository_server:/home/jcassidy/git-repos/BDPIDevice.git $git_repo_dir/BDPIDevice";
   # push @git_clone_commands, "git clone -b prasoon_dpimemcopyInterrupt $betzgrp_repository_server:/home/jcassidy/git-repos/CAPICMake.git $git_repo_dir/CAPICMake";
    #push @git_clone_commands, "git clone -b prasoon_dpimemcopyinterrupt $betzgrp_repository_server:/home/jcassidy/git-repos/BlueLink.git $git_repo_dir/BlueLink";
    push @git_clone_commands, "git clone -b rc2 $betzgrp_repository_server:/home/jcassidy/git-repos/FullMonteSW.git $git_repo_dir/FullMonteSW";
    #push @git_clone_commands, "git clone -b branch_yasmin $betzgrp_repository_server:/home/jcassidy/git-repos/FullMonte.git $git_repo_dir/FullMonteHW";

    for(@git_clone_commands){
        my $out = `$_`;
        print "$_\n$out\n";
    }

    return;
}

sub build_fmsr {
    # build fullmonte style repository (i.e. from BetzGroup as opposed to say pslse, a repository from IBM)
    (my $repo_name, my $cmake_cmd) = @_;
    my $make_cmd = "make >> make_stdout.txt 2>> make_stderr.txt";

    &HEADER($repo_name);
    chdir("$git_repo_dir/$repo_name");

    # Re-build if the repository exists
    if(-d "Build/Release"){
        chdir("Build/Release");
    }
    else{
        &mcd("Build");
        &mcd("Release");

        $cmake_cmd =~ s/\R/ /g;
        $cmake_cmd =~ s/ +/ /g;

        print "executing $cmake_cmd\n";
        `$cmake_cmd`;
    }

    #FMHW build has an error: it doesn't build the modules listed below, so we have to make them manually and then continue with the build process
    #if($repo_name eq "FullMonteHW"){
   #     print "executing $make_cmd\n";
    #    `$make_cmd`;

   #     my $module_make_command = "make Accumulator BRAMAccumulator Absorber > module_make_stdout.txt 2> module_make_stderr.txt";
    #    print "module-make command: $module_make_command\n"; 
    #    my $out = `$module_make_command`;
    #}
   # else{;}

    #=====================
    #Parallel Make Process
    #=====================
    # `make -k -j8` should work without requiring `make` after it
    # in many modules it doesn't, which means the dependencies are not properly specified
    #
    #BDPIDevice
    #   `make -k -j8` is 3s vs 11s of `make`; negligible speedup at the moment
    #
    #BlueLink
    # after a `make clean`
    #   `make`                      80s
    #   `make -k -j8`               doesn't work
    #   `make -k -j8` then `make`   30s
    # the last one is harder to implement and debug so we'll stick with `make` for now
    #
    #FMSW
    # `make`    140s
    # `make -k -j8` 23s + `make` 2.4s
    #
    #FMHW
    # `make` `make Accumulator BRAMAccumulator Absorber` `make`
    #   20s + 5s + 170s  
    # `make` `make Accumulator BRAMAccumulator Absorber` `make -k -j8` `make`
    #   20s + 5s + 20s + 5s

    if($repo_name eq "BlueLink" or $repo_name eq "FullMonteSW" or $repo_name eq "FullMonteHW"){
        my $parallel_make_cmd = "make -k -j8 >> makej_stdout.txt 2>> makej_stderr.txt";

        print "executing $parallel_make_cmd\n";
        `$parallel_make_cmd`;
    }

    print "executing $make_cmd\n";
    `$make_cmd`;

    chdir("$git_repo_dir");
    return;
}

sub build_for_simulation {
    &HEADER("simulation build");

    (my %params) = @_;
    my $boost_dir = $params{boost_dir};
    my $antlr_dir = $params{antlr_dir};

    # vsim is a binary file in modelsim
    my $modelsim_dir = `which vsim`;
    $modelsim_dir =~ s/\/bin\/vsim\n//;

    
    
    #FULLMONTE_SW
    my $repo_name = "FullMonteSW";

    my $cmake_cmd = "cmake $git_repo_dir/FullMonteSW 
    -DCMAKE_BUILD_TYPE=Release
    -DCMAKE_MODULE_PATH='$git_repo_dir/FullMonteSW;$git_repo_dir/FullMonteSW/cmake'
    -DARCH=AVX2
    -DWRAP_TCL=ON
    -DWRAP_VTK=ON
    -DANTLR3C_ROOT=$antlr_dir/install
    -DBOOST_ROOT=$boost_dir/install
    -DANTLR3_JAR=$antlr_dir/antlr-3.5.2-complete.jar
    > cmake_stdout.txt 2> cmake_stderr.txt";

    &build_fmsr($repo_name, $cmake_cmd);

    

    return;
}

sub build_for_synthesis {
    &HEADER("synthesis build. Nothing here yet");
    return;
}

sub parse_config_file{
    (my $fn) = @_;
    print "Looking for configuration file $fn\n";

    my %config_file;
    open(my $fh, "<", $fn) or die "failed to open $fn\n";
    while(<$fh>){
        chomp($_);
        s/\s//g;
        /\w+:[\w\/\\]+/ or next;
        (my $key, my $val) = split(":", $_);
        $config_file{$key} = $val;
    }

    my $timestamp = `date +%Y_%m_%d-%H_%M_%S_%Z`;
    chomp($timestamp);

    my $boost_dir = ($config_file{boost_dir} // "$original_dir/boost");
    my $antlr_dir = ($config_file{antlr_dir} // "$original_dir/antlr");

    my %params = (  boost_dir => $boost_dir,
                    antlr_dir => $antlr_dir,
                    timestamp => $timestamp,
                    );

    return(%params);
}

sub main {
    my $fn = "fm_build.config";
    $ARGV[0] and $fn = $ARGV[0];
    my %params = &parse_config_file($fn);

    (my $sysname, my $nodename, my $release, my $version, my $machine) = uname();
    #$nodename eq "betzgrp-pchenry" or die "This script only works from PC-HENRY (not $nodename)\n";

    # on PC-HENRY you might forget to load your modules, in which case the cmake version will be wrong
    # printing the version to STDOUT is an easy reminder that you need to load your modules
    #[???] can make a function that tests environment variables properly
    my $module_test = `cmake --version`;
    print "$module_test\nMake sure you have your modules loaded!\n";

    if(-d $git_repo_dir){
        print "directory '$git_repo_dir' already exists. We will assume repositories were cloned correctly and attempt a build\n";
    }
    else{
        &clone_git_repositories();
    }

    if($machine eq "ppc64le"){
        &build_for_synthesis(%params);
    }
    elsif($machine eq "x86_64"){
        &build_for_simulation(%params);
    }
    else{
        print "machine $machine not recognized. Exiting build script\n";
    }
    
    return;
}

=pod
=head1 Program Overview
This script is designed to build all of FullMonte and dependencies on PC-HENRY (Linux-x86)
"[???]" in the source means it's something I need to look at
=head2 Acronyms
fn  filename
fh  filehandle
fm  fullmonte
=head3 Before Running the Program
Before running the program you need the following modules
    bluespec
    altera-acds/13.1
    cmake
You will also need an install of ANTLR and BOOST in the directory from which you launch this script

Create a parameters file called "fm_build.config" with the locations of your ANTLR and BOOST installs.
    An example is provided: "fm_build.config" in this folder.
=head4 Required Enhancements
Some changes to source repositories to simplify build process
Commenting the source code
=cut
