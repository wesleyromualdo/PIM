<script type="text/javascript">
<?php
if ( $this->msgPositiva ){
?>
	swal("", '<?php echo $this->msgPositiva; ?>', "success");
//	alert('<?php echo $this->msgPositiva; ?>');	
<?php
}
?>
	
<?php
if ( $this->msgNegativa ){
?>
	swal("", '<?php echo $this->msgNegativa; ?>', "error");
//	alert('<?php echo $this->msgNegativa; ?>');	
<?php
}
?>
	
<?php
if ( $this->msgInformativa ){
?>
	swal("", '<?php echo $this->msgInformativa; ?>', "info");
//	alert('<?php echo $this->msgInformativa; ?>');	
<?php
}
?>
</script>