
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN">
<HTML>
<HEAD>
  <TITLE>Graphics formats supported by IVCon</TITLE>
  <META HTTP-EQUIV="Content-Type" CONTENT="text/html; charset=ISO-8859-1">

<?php
   require_once '../include/basics.php';
   $home_path="../";
   $file_ext="OBJ";
   addGlobalHeaderTags();
?>

</HEAD>

<?php
   require_once $home_path.'include/sidebar.php';
?>

<BODY TOPMARGIN=0 LEFTMARGIN=0 MARGINWIDTH=0 MARGINHEIGHT=0>

<div class="big_hdr" id="titlebar">Supported 3D formats</div>

<div id="toplogo"><img <?php echo "src=\"".$home_path."/pictures/ivcon_icon_64.png\""; ?>
 ALT="IVCon logo" TITLE="IVCon logo"/></div>

<?php drawSidebar('sidebar_formt','format_obj'); ?>

<div id="page_content">

<h3><?php echo $file_ext; ?> format overview:</h3>
<p>
Object files define the geometry and other properties for objects in
Wavefront's Advanced Visualizer. Object files can also be used to
transfer geometric data back and forth between the Advanced Visualizer
and other applications.<br>
Object files can be in ASCII format (.obj) or binary format (.mod).
</p>
<hr>

<h3><?php echo $file_ext; ?> file characteristics:</h3>
<p>
  <ul>
  <li>ASCII text</li>
  <li>3D vertices data</li>
  <li>Faces and meshes definition</li>
  <li>Lines definition</li>
  <li>No cameras or lights definition (not for sure)</li>
  <li>Color and textured</li>
  <li>No compression</li>
  </ul>
</p>
<hr>

<h3><?php echo $file_ext; ?> format references and information:</h3>
<p>
  <a href="http://people.scs.fsu.edu/%7Eburkardt/data/obj/obj.html">
  OBJ format information and examples</a> from John Burkardt
</p>
<hr>

<h3><?php echo $file_ext; ?> support notes inside IVCon source code:</h3>
<p>
<?php printCodeComment($home_path."/src/obj.cpp",256); ?>
</p>

<hr>
<?php addGlobalFooterTags(); ?>

</div>

<BR>

</BODY>
</HTML>
 