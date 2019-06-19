jQuery(document).ready(function ()
{
	jQuery('.ibox-footer .novo').click(function ()
	{ 
		location.href='exemplo.php?modulo=exemplo/controle/pessoa-teste/cadastrar';
	});
	
	jQuery('.ibox-footer .limpar').click(function ()
	{ 
		location.href='exemplo.php?modulo=exemplo/controle/pessoa-teste/listar';
	});
	
	jQuery('.ibox-footer .filtrar').click(function ()
	{
		jQuery('#form_filtro').submit();
	});
	
	jQuery('.js-collapse-link').click(function ()
	{
		$jsCollapseLink = this;
		jQuery($jsCollapseLink).parents('.ibox').find('.ibox-content').slideToggle(function()
		{
			jQuery('i', $jsCollapseLink).toggleClass('fa-chevron-down');
		});
	});
	
	
});

function editarPessoa(petid)
{
	location.href='?modulo=exemplo/controle/pessoa-teste/cadastrar&petid=' + petid;
}

function removerPessoa(petid)
{
	swal({
        title		: "Deseja apagar?",
        text		: "Se você clicar em \"Sim\" irá apagar o registro dessa pessoa.",
        type		: "warning",
        html		: true,
        showCancelButton	: true,
        cancelButtonText	: "Não",
        confirmButtonText	: "Sim",
        closeOnConfirm		: false,
        confirmButtonColor	: "#DD6B55"
    }, 
    function (isConfirm) 
    {
        if (isConfirm) {
        	location.href='?modulo=exemplo/controle/pessoa-teste/listar&requisicao=apagarPessoa&petid=' + petid;
        }
    });	
}