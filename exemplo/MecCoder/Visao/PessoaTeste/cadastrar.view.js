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
	jQuery('.salvar').click(function ()
	{
		validacaoAutomatica.enviarFormulario('formSalvarPessoa', 'requisicao', 'salvarPessoa');
	});
	
	jQuery('.novo').click(function ()
	{		
		location.href = '?modulo=exemplo/controle/pessoa-teste/cadastrar';
	});
});

