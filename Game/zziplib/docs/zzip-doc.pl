#! /usr/bin/env perl
eval 'exec perl -S $0 ${1+"$@"}'
    if 0;

use strict "var";

my $x;
my $F;
my @regs;
my %file;
my %func;

my %o;

my %fn;
my $id = 1000;

for $F (@ARGV)
{
    if ($F =~ /^(\w+)=(.*)/)
    {
	$o{$1} = $2;
    }else{
	open F, "<$F" or next;
	my $T = join ("",<F>); close F;
    
	$T =~ s/\&/\&amp\;/sg;
	$T =~ s/¬/\&#AC\;/sg;
	$T =~ s/\*\//¬/sg;
    
	while ( $T =~
		s/ \/\*\*(?=\s) ([^¬]+) ¬ ([^{}]+) \{ 
		/ push_regs($F," ".$1,$2) /gsex
		) {}
	    
	if ( $T =~ /^ \/\**(?=\s) ([^¬]+) ¬/sx)
	{
	    $file{$F}{comment} = $1;
	}
    }
}

sub push_regs
{
    my ($filename, $comment, $prototype) = @_;
 
    $comment =~ s/¬/\*\//;
    $func{$id}{filename} = $filename;
    $func{$id}{comment} = $comment;
    $func{$id}{prototype} = $prototype;
    $id ++;
    return $prototype;
}


if (not length ${specfile})
{
    $o{specfile} = "../zziplib.spec" if -f "../zziplib.spec";
    $o{specfile} = "zziplib.spec" if -f "zziplib.spec";
    $o{specfile} = "zziplib.spec" if not length $o{specfile};
}

sub getspecfile { 
    return `sed -e "/^$_[0] *:/!d" -e "s/^$_[0] *: *//" -e "q" $o{specfile}`;
}

$o{package} = getspecfile("[Nn]ame") if not length $o{package};
$o{package} = "a" if not length $o{package};
$o{package} =~ s{\s*}{}gs;

$o{version} = getspecfile("[Vv]ersion") if not length $o{version};
$o{version} =~ s{\s*}{}gs;

$o{dbkext} = "xml" if not length $o{xmlext};
$o{htmext} = "htm" if not length $o{htmext};
$o{serial} = "1" if not length $o{serial};
$o{generate} = "$o{package}$o{serial}" if not length $o{generate};
$o{docbookfile} = "$o{generate}.$o{dbkext}" if not length $o{docbookfile};
$o{libhtmlfile} = "$o{generate}.$o{htmext}" if not length $o{libhtmlfile};

# -----------------------------------------------------------------------

for $name (keys %file)
{
    $file{$name}{comment} =~ s/ ^\s?\s?\s?[^\*\s] (.*)$/ <pre> $1 <\/pre> /mgs;
    $file{$name}{comment} =~ s/ ^\s*[*]\s* $/ <p> /gmx;
    $file{$name}{comment} =~ s/ ^\s?\s?\s?\* (.*) $/ $1 /gmx;
    $file{$name}{comment} =~ s/<\/pre>([^<>]*)<pre>/$1/gs;
    $file{$name}{comment} =~ s/<([^<>]*\@[^<>]*)>/<email>$1<\/email>/gs;
    $file{$name}{comment} .= "<p>";

    $file{$name}{comment} =~ s{ [Aa]uthor\s*:(.*<\/email>) }
    {
	$file{$name}{author} = $1;
	"<author>".$1."</author>"
    }sex;

    $file{$name}{comment} =~ s{ [C]opyright[\s:]([^<>]*)<p> }
    {
	$file{$name}{copyright} = $1;
	"<copyright>".$1."</copyright>"
    }sex;
}

# -----------------------------------------------------------------------

my $V = "";
my $H = "";

my @namelist;
for $x (sort keys %func)
{
    my $name = $func{$x}{prototype};
    $name =~ s/^.*\b(\w+)\b\s*\(.*$/$1/s;
    push @namelist, $name;

    $func{$x}{prototype} =~ m{ ^(.*) \b(\w+)\b (\s*\(.*) $ }sx; 
    $fn{$name}{prespec} = $1; 
    $fn{$name}{namespec} = $2; 
    $fn{$name}{callspec} = $3;

    $fn{$name}{comment} = $func{$x}{comment};
    $fn{$name}{comment} =~ s/(^|\s)\=\>\"([^\"]*)\"/$1<link>$2<\/link>/gmx;
    $fn{$name}{comment} =~ s/(^|\s)\=\>\'([^\"]*)\'/$1<link>$2<\/link>/gmx;
    $fn{$name}{comment} =~ 
	s/(^|\s)\=\>\s([^\s\,\.\!\?\:\;\<\>\&\'\=\-]+)/$1<link>$2<\/link>/gmx;

    $fn{$name}{describe} = $fn{$name}{comment};
    $fn{$name}{describe} =~ s{^([^\n]*\n).*}{$1}gs;
    $fn{$name}{comment} =~ s{^[^\n]*\n}{}gs;
    $fn{$name}{describe} = "(".$func{$x}{filename}.")" 
	if $fn{$name}{describe} =~ /^\s*$/s;

    $fn{$name}{comment} =~ s/ ^\s?\s?\s?[^\*\s] (.*) $/ <pre> $1 <\/pre> /mgs;
    $fn{$name}{comment} =~ s/ ^\s?\s?\s?\* (.*) $/ <br \/> $1 /gmx;
    $fn{$name}{comment} =~ s/ ^\s*<br\s*\/>\s* $/ <p> /gmx;
    $fn{$name}{comment} =~ s/ (<p>\s*)<br\s*\/?>/$1/gsx;
    $fn{$name}{comment} =~ s/ (<p>\s*)<br\s*\/?>/$1/gsx;
    $fn{$name}{comment} =~ s/ (<br\s*\/?>\s*)<br\s*\/?>/$1/gsx;
    $fn{$name}{comment} =~ s/<c>/<code>/gs;
    $fn{$name}{comment} =~ s/<\/c>/<\/code>/gs;
    $fn{$name}{comment} =~ s/<\/pre>([^<>]*)<pre>/$1/gs;
    
    $fn{$name}{filename} = $func{$x}{filename};
    $fn{$name}{callspec} =~ s{^\s*}{}gsex;
    $fn{$name}{prespec}  =~ s{^\s*}{}gsex;
    $fn{$id} = $x;
}

for $name (@namelist) # <paramdef>
{
    $fn{$name}{callspec} =~ s:^([^\(\)]*)\(:$1<parameters>\(<paramdef>:s;
    $fn{$name}{callspec} =~ s:\)([^\(\)]*)$:</paramdef>\)</parameters>$1:s;
    $fn{$name}{callspec} =~ s:,:</paramdef>,<paramdef>:gs;
    $fn{$name}{callspec} =~ s:<paramdef>(\s+):$1<paramdef>:gs;
    $fn{$name}{callspec} =~ s:(\s+)</paramdef>:</paramdef>$1:gs;
}

for $name (@namelist)
{
    $fn{$name}{declcode} =
        "<td valign=\"top\"><code>".$fn{$name}{prespec}."<\/code><\/td>"
	."<td valign=\"top\">&nbsp;&nbsp;</td>"
	."<td valign=\"top\"><a href=\"#$name\">"
	."\n <code>".$fn{$name}{namespec}."<\/code>\n"
	."<\/a><\/td>"
	."<td valign=\"top\">&nbsp;&nbsp;</td>"
	."<td valign=\"top\">".$fn{$name}{callspec}."<\/td>";

    $fn{$name}{implcode} =
        "<code>".$fn{$name}{prespec}."<\/code>"
	."\n <br \/><b><code>".$fn{$name}{namespec}."<\/code><\/b>"
	."\n &nbsp; <code>".$fn{$name}{callspec}."<\/code>";

    $fn{$name}{declcode} =~ s{\s+<paramdef>}{\n<nobr>}gs;
    $fn{$name}{implcode} =~ s{\s+<paramdef>}{\n<nobr>}gs;
    $fn{$name}{declcode} =~ s{<paramdef>}{<nobr>}gs;
    $fn{$name}{implcode} =~ s{<paramdef>}{<nobr>}gs;
    $fn{$name}{declcode} =~ s{</paramdef>}{</nobr>}gs;
    $fn{$name}{implcode} =~ s{</paramdef>}{</nobr>}gs;
    $fn{$name}{declcode} =~ s{<parameters>}{\n <code>}gs;
    $fn{$name}{implcode} =~ s{<parameters>}{\n <code>}gs;
    $fn{$name}{declcode} =~ s{</parameters>}{</code>\n}gs;
    $fn{$name}{implcode} =~ s{</parameters>}{</code>\n}gs;
}

my $combinedstyle = 1;

for $name (@namelist)
{
    if ($fn{$name}{describe} =~ /^ \s* <link>(\w+)<\/link> /sx)
    {
	if ($combinedstyle and exists $fn{$1})
	{
	    $fn{$name}{into} = $1;
	}
    }

    # and prepare for filling in $combinedstyle
    $fn{$name}{_anchors} = "<a name=\"$name\" />";
    $fn{$name}{_impcode} = "<code>".$fn{$name}{implcode}."</code>";
    $fn{$name}{_comment} = "<p> &nbsp;".$fn{$name}{describe}."\n";
    $fn{$name}{_comment} .= "<p>".$fn{$name}{comment};
}

for $name (@namelist)
{
    next if not exists $fn{$name}{into};
    my $into = $fn{$name}{into};
    $fn{$into}{_anchors} .= "<a name=\"$name\" />";
    $fn{$into}{_impcode} .= "<br />\n";
    $fn{$into}{_impcode} .= "<code>".$fn{$name}{implcode}."</code>";
    my $text = $fn{$name}{comment};
    $text =~ s/ (T|t)his \s (function|procedure) /$1."he ".$name." ".$2/gsex;
    $fn{$name}{_comment} .= "<p>".$text;
}

# generated zziplist.html entries
for $name (@namelist)
{
    $fn{$name}{_comment} =~ s/ (<p>\s*)<br\s*\/>/$1/gsx;

    $H .= "<tr valign=\"top\">\n".$fn{$name}{declcode}."</tr>";
    next if $combinedstyle and exists $fn{$name}{into};

    $V .=  "\n<dt>".$fn{$name}{_anchors}.$fn{$name}{_impcode}."<dt>";
    $V .= "\n<dd>".$fn{$name}{_comment};
    $V .= "\n<p align=\"right\"><small>("
	.$fn{$name}{filename}.")</small></p></dd>";
}

$V =~ s/ <link>(\w+)([^<>]*)<\/link> / &a_name($1,$2) /gsex;
sub a_name 
{ 
    my ($n,$m) = @_; 
    if (exists $fn{$n}) { return "<a href=\"#$n\"><code>$n$m</code></a>"; }
    else { return "<code>$n$m</code>"; }
}
$V =~ s/ \-\> /<small>-\&gt\;<\/small>/gsx;

open F, ">$o{libhtmlfile}" or die "could not open '$o{libhtmlfile}': $!";
print F "<html><head><title> zziplib autodoc documentation </title></head>\n<body>\n";
print F "\n<h1>",$o{package}," <small><small><i>-", $o{version};
print F "</i></small></small></h1>";
print F "\n<table border=0 cellspacing=2 cellpadding=0>";
print F $H;
print F "\n</table>";
print F "\n<h3>Documentation</h3>\n";
print F "\n<dl>";
print F $V;
print F "\n</dl>";
print F "\n</body></html>\n";
close F;

# ======================================================================== #

my @headerlist;
my @mergedlist;

for $name (@namelist)
{
    push @headerlist, $name if not exists $fn{$name}{into};
    push @mergedlist, $name if exists $fn{$name}{into};
    $fn{$name}{_refhint} = 
		"\n<!--========= ".$name." (3) ===========-->\n";
    $fn{$name}{_refstart} = "";
    $H = $o{version}; $H =~ s{\s*}{}gs;
    $fn{$name}{_refentryinfo} = "\n <date>".$H."</date>";
    $H = $o{package}; $H =~ s{\s*}{}gs;
    $fn{$name}{_refentryinfo} .= "\n <productname>".$H."</productname>";
#    if (exists $file{ $fn{$name}{filename} }{author})
#    {
#	$H = $file{ $fn{$name}{filename} }{author};
#	$H =~ s{ \s* ([^<>]*) (<email>[^<>]*</email>) }{
#	    $fn{$name}{_refentryinfo} .= "\n <author>".$1.$2."</author>";
#	    "" }gmex;
#    }
    $fn{$name}{_refmeta} = "";
    $fn{$name}{_refnamediv} = "";
    $fn{$name}{_mainheader} = "zziplib.h";
    $fn{$name}{_manvolnum} = "3";
    $fn{$name}{_funcsynopsisinfo} = "";
    $fn{$name}{_funcsynopsis} = "";
    $fn{$name}{_description} = "";
    $fn{$name}{_refends} = "";
}

push @headerlist, @mergedlist;

for $name (@headerlist)
{
    my $into = $name; my $me = 1;

    if (exists $fn{$name}{into})
    {
	$into = $fn{$name}{into}; $me = 0;
	$fn{$name}{_refhint} =
	    "\n              <!-- see ".$fn{$name}{mergeinto}." -->\n";
    }

    $fn{$into}{_refstart} .= '<refentry id="'.$name.'">' if $me;
    $fn{$into}{_refends}  .= "\n</refentry>\n" if $me;
    $H = $name; $H =~ s{\s*}{}gs;
    $fn{$name}{_refentryinfo} .= "\n <title>".$H."</title>" if $me;
    $fn{$into}{_refmeta} .= "\n <manvolnum>"
	.$fn{$name}{_manvolnum}."</manvolnum>" if $me;
    $fn{$into}{_refmeta} .= "\n <refentrytitle>"
	.$name."</refentrytitle>" if $me;

    $fn{$name}{_funcsynopsisinfo} = "\n".' #include &lt;'.
	$fn{$into}{_mainheader}.'&gt;' if $me;

    $fn{$into}{_refnamediv} .= "\n ".
	"<refpurpose>".$fn{$name}{describe}." </refpurpose>" if $me;
    $fn{$into}{_refnamediv} .= "\n".' <refname>'.$name.'</refname>';

    $fn{$into}{_funcsynopsis} .= "\n <funcprototype>";
    $fn{$into}{_funcsynopsis} .= "\n <funcdef>"
	.$fn{$name}{prespec}." <function>".$name."</function></funcdef>";
    $H = $fn{$name}{callspec};
    $H =~ s{<parameters>\s*\(}{ }gs;
    $H =~ s{\)\s*</parameters>}{ }gs;
    $H =~ s{</paramdef>\s*,\s*}{</paramdef>\n }gs;
    $fn{$into}{_funcsynopsis} .= "\n".$H." </funcprototype>";

    $H = "<para>\n".$fn{$name}{comment}."\n</para>";
    $H =~ s/ his \s function /"he ".$name." function"/gsex;
    $H =~ s{<p>}{"\n</para><para>\n"}gsex;
    $H =~ s{<br\s*/?>}{}gs;
    $H =~ s{(</?)em>}{$1emphasis>}gs;
    $H =~ s{<code>}{<userinput>}gs;
    $H =~ s{</code>}{</userinput>}gs;
    $H =~ s{<link>}{<function>}gs;
    $H =~ s{</link>}{</function>}gs;
#    $H =~ s{<ul>}{</para><itemizedlist>}gs;
#    $H =~ s{</ul>}{</itemizedlist><para>}gs;
#    $H =~ s{<li>}{<listitem><para>}gs;
#    $H =~ s{</li>}{</para></listitem>\n}gs;
    $H =~ s{<ul>}{</para><programlisting>\n}gs;
    $H =~ s{</ul>}{</programlisting><para>}gs;
    $H =~ s{<li>}{}gs;
    $H =~ s{</li>}{}gs;

    $fn{$into}{_description} .= $H;

    if (exists $file{ $fn{$name}{filename} }{author})
    {
	$H = $file{ $fn{$name}{filename} }{author};
	$fn{$into}{_authors} = "<itemizedlist>";
	$H =~ s{ \s* ([^<>]*) (<email>[^<>]*</email>) }{
	    $fn{$into}{_authors} .= "\n <listitem><para>".$1." ".$2."</para></listitem>";
	    "" }gmex;
	$fn{$into}{_authors} .= "</itemizedlist>";
    }
    if (exists $file{ $fn{$name}{filename} }{copyright})
    {
	$fn{$into}{_copyright} = "<screen>\n"
	    .$file{ $fn{$name}{filename} }{copyright}.""
	    ."</screen>\n";
    }
}

my %header;

open F, ">$o{docbookfile}" or die "could not open $o{docbookfile}: $!";
print F '<!DOCTYPE reference PUBLIC "-//OASIS//DTD DocBook XML V4.1.2//EN"';
print F "\n",'   "http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd">';
print F "\n\n",'<reference><title>Manual Pages</title>',"\n";
for $name (@namelist)
{
    print F $fn{$name}{_refhint};
    next if exists $fn{$name}{into};
    print F $fn{$name}{_refstart};
    print F "\n<refentryinfo>",               $fn{$name}{_refentryinfo}
    ,       "\n</refentryinfo>\n"   if length $fn{$name}{_refentryinfo};
    print F "\n<refmeta>",                    $fn{$name}{_refmeta}
    ,       "\n</refmeta>\n"        if length $fn{$name}{_refmeta};
    print F "\n<refnamediv>",                 $fn{$name}{_refnamediv}
    ,       "\n</refnamediv>\n"     if length $fn{$name}{_refnamediv};

    print F "\n<refsynopsisdiv>"    if length $fn{$name}{_funcsynopsis};
    print F "\n<funcsynopsisinfo>",           $fn{$name}{_funcsynopsisinfo}
    ,       "\n</funcsynopsisinfo>" if length $fn{$name}{_funcsynopsisinfo};
    print F "\n<funcsynopsis>",               $fn{$name}{_funcsynopsis}
    ,       "\n</funcsynopsis>"     if length $fn{$name}{_funcsynopsis};
    print F "\n</refsynopsisdiv>"   if length $fn{$name}{_funcsynopsis};
    print F "\n<refsect1><title>Description</title>", $fn{$name}{_description}
    ,       "\n</refsect1>"                 if length $fn{$name}{_description};
    print F "\n<refsect1><title>Author</title>",      $fn{$name}{_authors}
    ,       "\n</refsect1>"                 if length $fn{$name}{_authors};
    print F "\n<refsect1><title>Copyright</title>",   $fn{$name}{_copyright}
    ,       "\n</refsect1>"                 if length $fn{$name}{_copyright};
	
    print F $fn{$name}{_refends};

    # ----------------------------------

    $H = $fn{$name}{_mainheader};
    my $me = 0; $me = 1 if not exists $header{$H};
    $header{$H}{_refstart} = "\n<refentry id=\"".$H."\">" if $me;
    $header{$H}{_refends} = "\n</refentry>\n" if $me;
    $header{$H}{_refentryinfo} = $fn{$name}{_refentryinfo} if $me;
    $header{$H}{_refentryinfo} =~ 
	s/(<title>)([^<>]*)(<\/title>)/$1 the library $3/s if $me;
    $header{$H}{_refmeta} = "\n   <manvolnum>"
	.$fn{$name}{_manvolnum}."</manvolnum>\n" if $me;
    $header{$H}{_refmeta} .= "\n   <refentrytitle>"
	.$fn{$name}{_mainheader}."</refentrytitle>" if $me;
    $header{$H}{_refnamediv} = "\n   <refpurpose> library </refpurpose>";
    $header{$H}{_refnamediv} .= "\n   <refname>".$H."</refname>";

    $header{$H}{_refsynopsisinfo} = 
	$fn{$name}{_refsynopsisinfo} if exists $fn{$name}{_refsynopsisinfo};
    $header{$H}{_funcsynopsis} .= "\n"
	.$fn{$name}{_funcsynopsis} if exists  $fn{$name}{_funcsynopsis};
#    $header{$H}{_funcsynopsisdiv} .= "\n<funcsynopsis>"
#	.$fn{$name}{_funcsynopsis}."</funcsynopsis>"
#	if exists  $fn{$name}{_funcsynopsis};
    $header{$H}{_copyright} =
	$fn{$name}{_copyright} if exists  $fn{$name}{_copyright} and $me;
    $header{$H}{_authors} = 
	$fn{$name}{_authors}   if exists  $fn{$name}{_authors} and $me;
    if ($me)
    {
	my $T = `cat $o{package}.spec`;
	if ($T =~ /\%description\b([^\%]*)\%/s)
	{
	    $header{$H}{_description} = $1;
	}elsif (not length $header{$H}{_description})
	{
	    $header{$H}{_description} = "$o{package} library";
	}
    }
}

for $H (keys %header)
{
    next if not length $header{$H}{_refstart};
    print F "\n<!-- _______ ",$H," _______ -->";
    print F $header{$H}{_refstart};
    print F "\n<refentryinfo>",               $header{$H}{_refentryinfo}
    ,       "\n</refentryinfo>\n"   if length $header{$H}{_refentryinfo};
    print F "\n<refmeta>",                    $header{$H}{_refmeta}
    ,       "\n</refmeta>\n"        if length $header{$H}{_refmeta};
    print F "\n<refnamediv>",                 $header{$H}{_refnamediv}
    ,       "\n</refnamediv>\n"     if length $header{$H}{_refnamediv};

    print F "\n<refsynopsisdiv>"    if length $header{$H}{_funcsynopsis};
    print F "\n<funcsynopsisinfo>",           $header{$H}{_funcsynopsisinfo}
    ,       "\n</funcsynopsisinfo>" if length $header{$H}{_funcsynopsisinfo};
    print F "\n<funcsynopsis>",               $header{$H}{_funcsynopsis}
    ,       "\n</funcsynopsis>"     if length $header{$H}{_funcsynopsis};
    print F  "\n</refsynopsisdiv>"  if length $header{$H}{_funcsynopsis};

    print F "\n<refsect1><title>Description</title>", $header{$H}{_description}
    ,       "\n</refsect1>"                if length $header{$H}{_description};
    print F "\n<refsect1><title>Author</title>",      $header{$H}{_authors}
    ,       "\n</refsect1>"                 if length $header{$H}{_authors};
    print F "\n<refsect1><title>Copyright</title>",   $header{$H}{_copyright}
    ,       "\n</refsect1>"                 if length $header{$H}{_copyright};
	
    print F $header{$H}{_refends};
}
print F "\n",'</reference>',"\n";
close (F);

