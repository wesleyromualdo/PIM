<?php 
/**
 * Template básico de assinatura eletrônica do Simec. Feito com o mínimo de CSS possível.
 * 
 * Para criar seu próprio template apenas crie outro arquivo nesta mesma pasta 
 * e passe o nome como segundo parâmetro para o método imprimirAssinatura() da classe
 * Simec_Assinaturaeletronica_Assinatura().
 * 
 * informações disponíveis:
 * 
 * - $this->img é o caminho para a imagem na pasta www
 * - $this->nome é o nome do usuário que fez a assinatura 
 *      (Já tratado para apenas a primeira letra de cada nome ser maiúscula)
 * - $this->data() é uma função que exibe a data de assinatura de acordo com o 
 *      formtado da funação date() (padrão 'd/m/Y')
 * - $this->cpf é o CPF do usuário que fez a assinatura
 * 
 */

if( isset($this->cargo) && !empty($this->cargo) ){
    $strCargo = $this->cargo.", ";
}

//largura a altura padrão da tarja
$width = 630;
$height = 70;

//parametrizando altura 
if ( isset($altura) && !empty($altura) ) {
    $height = $altura;
}

//parametrizando largura
if ( isset($largura) && !empty($largura) ) {
    $width = $largura;
}

//incializando variáveis possíveis de parâmetro
$style      = !empty(trim($style))      ? $style : null;
$prefixo    = !empty(trim($prefixo))    ? $prefixo : null;
$textoextra = !empty(trim($textoextra)) ? $textoextra : null;
$posfixo    = !empty(trim($posfixo))    ? $posfixo : null;

?>
<style>
    .container_assinatura
    {
        margin: 0 auto;  
        width: <?=$width;?>px; 
        border:1px solid black;
        display: block;
        height: <?=$height;?>px;
    }
    
    .img_assinatura
    {
        float:left;
    }
    
    .container_texto_assinatura
    {
        padding-top: 15px;
    }
    
    <?php
        //imprimindo parâmetro adicional de style que permite mandar CSS como uma string
        echo  $style; 
    ?>
</style>

<div class="container_assinatura">
	<div class="img_assinatura">
		<img id="img_assinatura"  src="<?= $this->img; ?>" >
	</div>	
    <div class="container_texto_assinatura">
        <p>
        	<?= $prefixo; ?>
        	Documento assinado eletronicamente por <b><?= $this->nome; ?></b>,
        	<?= $strCargo; ?> 
        	em <?= $this->data(); ?>, às <?= $this->data('H:m:s');?>,
        	<br><br>
        	<?= $textoextra; ?>
        	conforme horário oficial de Brasília, com fundamento da Portaria nº 1.042/2015 do Ministério da Educação.
        	<?= $posfixo; ?>
        </p>
    </div>
</div>
