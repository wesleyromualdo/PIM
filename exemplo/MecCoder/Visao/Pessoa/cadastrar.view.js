validacaoAutomatica = new ValidacaoAutomatica();
validacaoAutomatica.addCampo('name', 'nome', 'Nome')
				   .addValidaObrigatorio();

validacaoAutomatica.addCampo('name', 'idade', 'Idade')
				   .addValidaObrigatorio()
				   .addValidaExterno(function () 
					{ 
					   return (isNaN(jQuery('[name=idade]').val()) ? 'O campo idade deve ser preenchido com um número inteiro.' : true); 
					});

validacaoAutomatica.addCampo('id', 'estuf', 'Estado')
				   .addValidaObrigatorio();

validacaoAutomatica.addCampo('name', 'sexo', 'Sexo')
				   .addValidaObrigatorio();

jQuery(document).ready(function ()
{
	jQuery('.ibox-footer .salvar').click(function ()
	{
		validacaoAutomatica.enviarFormulario('formSalvarPessoa', 'requisicao', 'salvarPessoa');
	});
	
	jQuery('.ibox-footer .novo').click(function ()
	{		
		location.href = '?modulo=exemplo/controle/pessoa/cadastrar';
	});
});

