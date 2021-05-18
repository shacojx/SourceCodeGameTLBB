<?php
# use strict "var"; # can anybody give me hint how to enforce that?


$o = array (
    docbookfile => "zziplib2.xml",
    libhtmlfile => "zziplib2.html",
    );

$o[version] = preg_replace('{\s*}s','',
    `grep -i "^version *:" zziplib.spec | sed -e "s/[Vv]ersion *: *//"`);
$o[package] = preg_replace('{\s*}s','',
    `grep -i "^name *:" zziplib.spec | sed -e "s/[Nn]ame *: *//"`);

# subject as first arg - looks more like assignment from-to with-regex
function reg_replace1 ($S,$P,$R) { return preg_replace ($P,$R,$S,1); }
function reg_replace ($S,$P,$R) { return preg_replace ($P,$R,$S); }
function reg_matched ($S,$P) { return preg_match ($P,$S); }
function reg_match ($S,$P,&$M) { return preg_match ($P,$S,$M); }
function unquote($S) { return str_replace('\"','"', $S); }

$id = 1000;
$MB = 1000000;

$func;

$F = "";
$file = array ();
$fn = array ();
foreach ($argv as $F)
{
    if (reg_match ($F, '{^(\w+)=(.*)}', $X)):
	$o[$X[1]] = $X[2];
    else:
	$H = fopen ($F,"r");
	if (! $H) continue;
	$T = fread ($H,$MB); fclose ($H);

	$T = str_replace ("&","&amp;",$T);
	$T = str_replace ("¬","&#AC;",$T);
	$T = str_replace ("*/","¬",$T);

	$T = reg_replace ($T,
            '{ [/][*][*](?=\s) ([^¬]+) ¬ ([^{}]+) \{ }sex',
            "push_regs('$F',' '.unquote('$1'),unquote('$2'))");
	    
	if (reg_match($T, '{ ^ [/][*]*(?=\s) ([^¬]+) ¬ }sx', $X))
	{
	    $file[$F][comment] = $X[1];
	}
    endif;
}

function push_regs ($filename, $comment, $prototype)
{
    global $func, $id;
    $comment = str_replace ("¬", "*/", $comment); 
    $func[$id][filename] = $filename;
    $func[$id][comment] = $comment;
    $func[$id][prototype] = $prototype;
    $id ++;
    return $prototype;
}

# in php you can not modify variables within an /e-val of a reg_replace
function set_file($a,$b,$c) { global $file; $file[$a][$b] = $c; }
function set_fn($a,$b,$c)   { global $fn;     $fn[$a][$b] = $c; }
function append_fn($a,$b,$c)   { global $fn;  $fn[$a][$b] .= $c; }

# -----------------------------------------------------------------------
foreach (array_keys($file) as $name)
{
    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ ^\s?\s?\s? [^\*\s] (.*)$ }mx','<pre> $1 </pre>');
    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ ^\s*[*]\s* $ }mx',' <p> ');
    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ ^\s?\s?\s?\* (.*) }mx',' $1 ');
    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ </pre> ([^<>]*) <pre> }sx','$1');
    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ <([^<>]* [@] [^<>]*)> }sx','<email>$1</email>');
    $file[$name][comment] .= "<p>";

    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ [Aa]uthor\s*:(.*</email>) }sex',
         "set_file('$name','author',unquote('$1')); 
              '<author>'.unquote('$1').'</author>'");

    $file[$name][comment] = reg_replace ($file[$name][comment],
         '{ [C]opyright[\s:]([^<>]*)<p> }sex',
         "set_file('$name','copyright',unquote('$1'));
              '<copyright>'.unquote('$1').'</copyright>'");
}

# -----------------------------------------------------------------------

$V = "";
$W = "";
$namelist = array ();

foreach ($func as $function)
{
    $name = reg_replace ($function[prototype],
           '{ ^.* \b(\w+)\b \s* \(.*$ }sx','$1');
    $namelist[] = $name;

    reg_match ($function[prototype],
        '{ ^ (.*) \b(\w+)\b (\s*\(.*) $ }sx', $X); 
    $fn[$name][prespec] =  reg_replace($X[1], '{^\s*}sx','');
    $fn[$name][namespec] = reg_replace($X[2], '{^\s*}sx','');
    $fn[$name][callspec] = reg_replace($X[3], '{^\s*}sx','');
    $fn[$name][filename] = $function[filename]; 

    $fn[$name][comment] = $function[comment];
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (^|\s) => "([^"]*)" }mx', "$1<link>$2</link>");
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (^|\s) => \'([^"]*)\' }mx', "$1<link>$2</link>");
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (^|\s) => \s([^\s,.!\?:;<>&\'\=\-]+) }mx', "$1<link>$2</link>");

    $fn[$name][describe] = reg_replace1 ($fn[$name][comment],
        '{ ^([^\n]*)\n.* }sx', '$1');
    $fn[$name][comment] = reg_replace1 ($fn[$name][comment],
        '{ ^[^\n]*\n }sx', '');
    if (reg_matched ($fn[$name][describe], '{ ^\s*$ }sx' )):
    $fn[$name][describe] = "(".$function[filename].")"; endif;

    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ ^\s?\s?\s?[^\*\s] (.*) $ }mx', ' <pre> $1 </pre> ');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ ^\s?\s?\s?\* (.*) $ }mx',' <br /> $1 ');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ ^\s*<br\s*/>\s* $ }mx',' <p> ');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (<p>\s*) <br\s*/?> }sx', '$1');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (<p>\s*) <br\s*/?> }sx', '$1');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ (<br\s*/?>\s*) <br\s*/?> }sx', '$1');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ <c> }sx', '<code>');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ </c> }sx', '</code>');
    $fn[$name][comment] = reg_replace ($fn[$name][comment],
        '{ </pre> ([^<>]*) <pre> }sx', '$1');
}

foreach ($namelist as $name) # <paramdef>
{
    $convert = array (
        '{ ^ ([^\(\)]*) \( }sx'   => '$1<parameters>(<paramdef>',
        '{ \) ([^\(\)]*) $ }sx'   => '</paramdef>)</parameters>$1',
        '{ , }sx'                 => '</paramdef>,<paramdef>',
        '{ <paramdef> (\s+) }sx'  => '$1<paramdef>',
        '{ (\s+) </paramdef> }sx' => '</paramdef>$1');
	
    $fn[$name][callspec] = reg_replace ($fn[$name][callspec],
	array_keys($convert), array_values($convert));
}

foreach ($namelist as $name)
{
    $fn[$name][declcode] =
        "<td valign=\"top\"><code>".$fn[$name][prespec]."</code></td>"
	."<td valign=\"top\">&nbsp;&nbsp;</td>"
	."<td valign=\"top\"><a href=\"#$name\">"
	."\n <code>".$fn[$name][namespec]."</code>\n"
	."</a></td>"
	."<td valign=\"top\">&nbsp;&nbsp;</td>"
	."<td valign=\"top\">".$fn[$name][callspec]."</td>";
    
    $fn[$name][implcode] =
        "<code>".$fn[$name][prespec]."</code>"
	."\n <br /><b><code>".$fn[$name][namespec]."</code></b>"
	."\n &nbsp; <code>".$fn[$name][callspec]."</code>";

    $convert = array (
	'{\s+<paramdef>}s' => '\n<nobr>',
	'{<paramdef>}s'    => '<nobr>',
	'{</paramdef>}s'   => '</nobr>',
	'{<parameters>}s'  => '<code>',
	'{<parameters>}s'  => '</code>');
	
    $fn[$name][declcode] = reg_replace ($fn[$name][declcode], 
			   array_keys($convert), array_values($convert));
    $fn[$name][implcode] = reg_replace ($fn[$name][implcode], 
			   array_keys($convert), array_values($convert));
}

$combinedstyle = 1;

foreach ($namelist as $name)
{
    if (reg_match ($fn[$name][describe], '{^ \s* <link>(\w+)</link> }sx', $X))
    {
	if ($combinedstyle and isset($fn[$X[1]]))
	{
	    $fn[$name][into] = $X[1];
	}
    }
    
# and prepare for filling in $combinedstyle
    $fn[$name][_anchors] = "<a name=\"$name\" />";
    $fn[$name][_impcode] = "<code>".$fn[$name][implcode]."</code>";
    $fn[$name][_comment] = "<p> &nbsp;".$fn[$name][describe]."\n";
    $fn[$name][_comment] .= "<p>".$fn[$name][comment];
}

foreach ($namelist as $name)
{
    if (!isset($fn[$name][into])) continue;
    $into = $fn[$name][into]; 
    $fn[$into][_anchors] .= "<a name=\"$name\" />";
    $fn[$into][_impcode] .= "<br />\n";
    $fn[$into][_impcode] .= "<code>".$fn[$name][implcode]."</code>";
    $fn[$name][_comment] .= "<p>"
	. reg_replace ($fn[$name][comment],
	'{ (T|t)his \s (function|procedure) }sx', '$1he $name $2');
}

# generated zziplist.html entries
foreach ($namelist as $name)
{
    $fn[$name][_comment] = reg_replace ($fn[$name][_comment],
					'{ (<p>\s*)<br\s*/> }sx', '$1');

    $W .= "<tr valign=\"top\">\n".$fn[$name][declcode]."</tr>";
    if ($combinedstyle and isset($fn[$name][into])) continue;
    
    $V .= "\n<dt>".$fn[$name][_anchors].$fn[$name][_impcode]."<dt>";
    $V .= "\n<dd>".$fn[$name][_comment];
    $V .= "\n<p align=\"right\"><small>("
	.$fn[$name][filename].")</small></p></dd>";
}

$V = reg_replace ($V, 
     '{ <link>(\w+)([^<>]*)</link> }sex', "a_name(\"$1\",\"$2\")");
function a_name ($n, $m)
{ 
    global $fn;
    if (isset($fn[$n])) { return "<a href=\"#$n\"><code>$n$m</code></a>"; }
    else { return "<code>$n$m</code>"; }
}
$V = reg_replace ($V, '{ \-\> }sx', '<small>-&gt;</small>');

$F = fopen ("$o[libhtmlfile]","w");
 if (!$F): die("could not open '$o[libhtmlfile]'");
else:
fputs ($F, "<html><head><title>".$o[package]
           ." autodoc documentation </title></head>\n<body>\n" );
fputs ($F, "\n<h1>".$o[package]." <small><small><i>-". $o[version]);
fputs ($F, "</i></small></small></h1>");
fputs ($F, "\n<table border=0 cellspacing=2 cellpadding=0>");
fputs ($F, $W);
fputs ($F, "\n</table>");
fputs ($F, "\n<h3>Documentation</h3>\n");
fputs ($F, "\n<dl>");
fputs ($F, $V);
fputs ($F, "\n</dl>");
fputs ($F, "\n</body></html>\n");
fclose($F);
endif;

# ======================================================================== #

$headerlist = array ();
$mergedlist = array ();

foreach ($namelist as $name)
{
    if (!isset($fn[$name][into])):
	$headerlist[]= $name; else:
        $mergedlist[]= $name; endif;
    $fn[$name][_refhint] = 
		"\n<!--========= ".$name." (3) ===========-->\n";
    $fn[$name][_refstart] = "";
    $W = reg_replace($o[version], '{\s*}s','');
    $fn[$name][_refentryinfo] = "\n <date>".$W."</date>";
    $W = reg_replace($o[package], '{\s*}s','');
    $fn[$name][_refentryinfo] .= "\n <productname>".$W."</productname>";
#    if (exists $file{ $fn[$name]{filename} }{author})
#    {
#	$W = $file{ $fn[$name]{filename} }{author};
#	$W =~ s{ \s* ([^<>]*) (<email>[^<>]*</email>) }{
#	    $fn[$name][_refentryinfo] .= "\n <author>".$1.$2."</author>";
#	    "" }gmex;
#    }
    $fn[$name][_refmeta] = "";
    $fn[$name][_refnamediv] = "";
    $fn[$name][_mainheader] = "zziplib.h";
    $fn[$name][_manvolnum] = "3";
    $fn[$name][_funcsynopsisinfo] = "";
    $fn[$name][_funcsynopsis] = "";
    $fn[$name][_description] = "";
    $fn[$name][_refends] = "";
}


foreach (array_merge($headerlist, $mergedlist) as $name)
{
    $into = $name; $me = 1; 

    if (isset($fn[$name][into]))
    {
	$into = $fn[$name][into]; $me = 0; 
	$fn[$name][_refhint] =
	    "\n              <!-- see ".$fn[$name][mergeinto]." -->\n";
    }

    if ($me):
    $fn[$into][_refstart] .= '<refentry id="'.$name.'">';
    $fn[$into][_refends]  .= "\n</refentry>\n";
    $W = reg_replace($name, '{\s*}s',"");
    $fn[$name][_refentryinfo] .= "\n <title>".$W."</title>";
    $fn[$into][_refmeta] .= "\n <manvolnum>"
	.$fn[$name][_manvolnum]."</manvolnum>";
    $fn[$into][_refmeta] .= "\n <refentrytitle>"
	.$name."</refentrytitle>";

    $fn[$name][_funcsynopsisinfo] = "\n".' #include &lt;'.
	$fn[$into][_mainheader].'&gt;';

    $fn[$into][_refnamediv] .= "\n ".
	"<refpurpose>".$fn[$name][describe]." </refpurpose>";
    endif; # $me
    $fn[$into][_refnamediv] .= "\n".' <refname>'.$name.'</refname>';

    $fn[$into][_funcsynopsis] .= "\n <funcprototype>";
    $fn[$into][_funcsynopsis] .= "\n <funcdef>"
	.$fn[$name][prespec]." <function>".$name."</function></funcdef>";
    $W = $fn[$name][callspec];
    $convert = array (
       '{<parameters>\s*\(}s' => " ",
       '{\)\s*</parameters>}s' => " ",
       '{</paramdef>\s*,\s*}s' => "</paramdef>\n " );
    $fn[$into][_funcsynopsis] .= "\n".reg_replace ($W,
        array_keys($convert), array_values($convert))." </funcprototype>";

    $convert = array ( 
        '{ (T|t)his \s (function|procedure) }sx' => "$1he $name $2",
        '{ <p> }sx' => "\n</para><para>\n",
        '{<br\s*/?>}s' => "",
        '{(</?)em>}s' => "$1emphasis>",
        '{<code>}s' => "<userinput>",
        '{</code>}s' => "</userinput>",
        '{<link>}s' => "<function>",
        '{</link>}s' => "</function>",
#       '{<ul>}s' => "</para><itemizedlist>",
#       '{</ul>}s' => "</itemizedlist><para>",
#       '{<li>}s' => "<listitem><para>",
#       '{</li>}s' => "</para></listitem>\n",
        '{<ul>}s' => "</para><programlisting>\n",
        '{</ul>}s' => "</programlisting><para>",
        '{<li>}s' => "",
        '{</li>}s' => "" );

    $fn[$into][_description] .= reg_replace (
         "<para>\n".$fn[$name][comment]."\n</para>",
         array_keys($convert), array_values($convert));

    if (isset( $file[ $fn[$name][filename] ][author] ))
    {
	$W = $file[ $fn[$name][filename] ][author];
	$fn[$into][_authors] = "<itemizedlist>";
	reg_replace ($W, '{ \s* ([^<>]*) (<email>[^<>]*</email>) }mex',
            "append_fn('$into','_authors', '\n <listitem><para>'.unquote('$1')
                       .' '.unquote('$2').'</para></listitem>');''");
	$fn[$into][_authors] .= "</itemizedlist>";
    }
    if (isset( $file[ $fn[$name][filename] ][copyright] ))
    {
	$fn[$into][_copyright] = "<screen>\n"
	    .$file{ $fn[$name][filename] }[copyright].""
	    ."</screen>\n";
    }
}

$header = array (); # hash

$F = fopen ("$o[docbookfile]","w");
if (!$F): die ("could not open $o[docbookfile]:");
else: 
fputs($F,'<!DOCTYPE reference PUBLIC "-//OASIS//DTD DocBook XML V4.1.2//EN"');
fputs($F,"\n   \"http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd\">");
fputs($F, "\n\n".'<reference><title>Manual Pages</title>'."\n");
foreach ($namelist as $name)
{
    fputs($F, $fn[$name][_refhint] );
    if (isset( $fn[$name][into] )) continue;

    fputs($F, $fn[$name][_refstart] );
    if (@strlen( $fn[$name][_refentryinfo] )):
    fputs($F, "\n<refentryinfo>");
    fputs($F, $fn[$name][_refentryinfo]);
    fputs($F, "\n</refentryinfo>\n" ); endif;
    if (@strlen( $fn[$name][_refmeta] )):
    fputs($F, "\n<refmeta>");
    fputs($F,  $fn[$name][_refmeta] ); 
    fputs($F,  "\n</refmeta>\n"); endif;
    if (@strlen( $fn[$name][_refnamediv] )):
    fputs($F, "\n<refnamediv>");
    fputs($F,  $fn[$name][_refnamediv] );
    fputs($F,  "\n</refnamediv>\n"); endif;

    if (@strlen( $fn[$name][_funcsynopsis])):
    fputs($F, "\n<refsynopsisdiv>"); endif;
    if (@strlen($fn[$name][_funcsynopsisinfo])):
    fputs($F, "\n<funcsynopsisinfo>");
    fputs($F,  $fn[$name][_funcsynopsisinfo]);
    fputs($F, "\n</funcsynopsisinfo>"); endif;
    if (@strlen($fn[$name][_funcsynopsis])):
    fputs($F, "\n<funcsynopsis>");
    fputs($F,  $fn[$name][_funcsynopsis]);
    fputs($F, "\n</funcsynopsis>"); endif;
    if (@strlen( $fn[$name][_funcsynopsis])):
    fputs($F, "\n</refsynopsisdiv>"); endif;

    if (@strlen( $fn[$name][_description])):
    fputs($F, "\n<refsect1><title>Description</title>");
    fputs($F, $fn[$name][_description]);
    fputs($F, "\n</refsect1>"); endif;
    if (@strlen( $fn[$name][_authors])):
    fputs($F, "\n<refsect1><title>Author</title>");
    fputs($F, $fn[$name][_authors]);
    fputs($F, "\n</refsect1>"); endif;
    if (@strlen( $fn[$name][_copyright])):
    fputs($F, "\n<refsect1><title>Copyright</title>");
    fputs($F, $fn[$name][_copyright]);
    fputs($F, "\n</refsect1>"); endif;

    fputs($F, $fn[$name][_refends]);

    # ----------------------------------

    $W = $fn[$name][_mainheader];
    $me = 0; if (!isset($header[$W])) $me = 1;
    if ($me):
    $header[$W][_refstart] = "\n<refentry id=\"".$W."\">";
    $header[$W][_refends] = "\n</refentry>\n";
    $header[$W][_refentryinfo] = reg_replace ($fn[$name][_refentryinfo],
	'{ (<title>) ([^<>]*) (</title>) }sx', '$1 the library $3');
    $header[$W][_refmeta] = "\n   <manvolnum>".$fn[$name][_manvolnum]
	."</manvolnum>\n";
    $header[$W][_refmeta] .= "\n   <refentrytitle>".$fn[$name][_mainheader]
	."</refentrytitle>"; 
    endif;
    $header[$W][_refnamediv] = "\n   <refpurpose> library </refpurpose>";
    $header[$W][_refnamediv] .= "\n   <refname>".$W."</refname>";

    if (isset($fn[$name][_refsynopsisinfo])):
    $header[$W][_refsynopsisinfo] = $fn[$name][_refsynopsisinfo]; endif;
    if (isset($fn[$name][_funcsynopsis])):
    $header[$W][_funcsynopsis] .= "\n".$fn[$name][_funcsynopsis]; endif;
#   if (isset( $fn[$name][_funcsynopsis])):
#    $header[$W][_funcsynopsisdiv] .= "\n<funcsynopsis>"
#	.$fn[$name][_funcsynopsis]."</funcsynopsis>"
    if (isset($fn[$name][_copyright]) and $me)
	$header[$W][_copyright] = $fn[$name][_copyright];
    if (isset($fn[$name][_authors]) and $me)
	$header[$W][_authors] = $fn[$name][_authors];

    if ($me)
    {
	if (reg_match (`cat "$o[package].spec"`,
		       '{%description\b([^%]*)%}s', $X))
	{
	    $header[$W][_description] = $X[1];
	} elseif (!@strlen( $header[$W][_description] ))
	{
	    $header[$W][_description] = "$o[package] library";
	}
    }
}

foreach (array_keys ($header) as $W)
{
    if (! @strlen($header[$W][_refstart])) continue;

    fputs($F, "\n<!-- _______ ".$W." _______ -->");
    fputs($F, $header[$W][_refstart]);

    if (@strlen( $header[$W][_refentryinfo] )):
    fputs($F, "\n<refentryinfo>");
    fputs($F, $header[$W][_refentryinfo]);
    fputs($F, "\n</refentryinfo>\n" ); endif;
    if (@strlen( $header[$W][_refmeta] )):
    fputs($F, "\n<refmeta>");
    fputs($F, $header[$W][_refmeta]);
    fputs($F, "\n</refmeta>\n" ); endif;
    if (@strlen( $header[$W][_refnamediv] )):
    fputs($F, "\n<refnamediv>");
    fputs($F, $header[$W][_refnamediv]);
    fputs($F, "\n</refnamediv>\n" ); endif;

	  
    if (@strlen($header[$W][_funcsynopsis])):
    fputs($F, "\n<refsynopsisdiv>"); endif;
    if (@strlen( $header[$W][_funcsynopsisinfo] )):
    fputs($F, "\n<funcsynopsisinfo>");
    fputs($F, $header[$W][_funcsynopsisinfo]);
    fputs($F, "\n</funcsynopsisinfo>\n" ); endif;
    if (@strlen( $header[$W][_funcsynopsis] )):
    fputs($F, "\n<funcsynopsis>");
    fputs($F, $header[$W][_funcsynopsis]);
    fputs($F, "\n</funcsynopsis>\n" ); endif;
    if (@strlen($header[$W][_funcsynopsis])):
    fputs($F, "\n</refsynopsisdiv>"); endif;

    if (@strlen( $header[$W][_description])):
    fputs($F, "\n<refsect1><title>Description</title>");
    fputs($F, $header[$W][_description]);
    fputs($F, "\n</refsect1>"); endif;
    if (@strlen( $header[$W][_authors])):
    fputs($F, "\n<refsect1><title>Author</title>");
    fputs($F, $header[$W][_authors]);
    fputs($F, "\n</refsect1>"); endif;
    if (@strlen( $header[$W][_copyright])):
    fputs($F, "\n<refsect1><title>Copyright</title>");
    fputs($F, $header[$W][_copyright]);
    fputs($F, "\n</refsect1>"); endif;
	
    fputs($F, $header[$W][_refends]);
}
fputs($F, "\n".'</reference>'."\n");

endif; fclose ($F);
