<?php 

/*
 código para popup que exibe todas as obras relacionadas a um número de processo.
 Utilizado na lista de Cumprimiento de Objeto do Obras 2.0
*/

// carrega as bibliotecas internas do sistema
include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once "_constantes.php";
include_once "_funcoes.php";

//abrindo conexão com o banco de dados
$db = new cls_banco();

//obtendo o número do processo
$processo = trim(filter_input(INPUT_GET, 'processo'));


if(empty($processo)){
	die('Número do processo não informado.');
}

$sql = "
		SELECT 
			'<a href=\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obra.obrid || '\">' || obra.obrid || '</a>' AS id, 
			'<a href=\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obra.obrid || '\">' || obra.obrnome || '</a>' AS nome, 
			'<a href=\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obra.obrid || '\">' || municipio.mundescricao || '</a>' AS municipio, 
			'<a href=\"/obras2/obras2.php?modulo=principal/cadObra&acao=A&obrid=' || obra.obrid || '\">' || endereco.estuf || '</a>' AS uf  

		FROM obras2.vm_termo_convenio_obras As processo
		
		JOIN obras2.obras AS obra ON (processo.obrid = obra.obrid)
		JOIN entidade.endereco AS endereco ON (obra.endid = endereco.endid)
		JOIN territorios.municipio AS municipio ON (endereco.muncod = municipio.muncod)

		WHERE processo.pronumeroprocesso = '{$processo}'";
		
$cabecalho = array('ID', 'Nome da Obra', 'Município', 'UF');
?>
<html>
<head>
    <title>SIMEC- Sistema Integrado de Monitoramento do Ministério da Educação</title>
    <script language="JavaScript" src="../../../includes/funcoes.js"></script>
    <link rel="stylesheet" type="text/css" href="../../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../../includes/listagem.css">
    <meta http-equiv="Content-Type" content="text/html;charset=utf-8">
</head> 

<body topmargin="0" leftmargin="0">
	<div>
		<?php $db->monta_lista($sql,$cabecalho,30,5,'N','center'); ?>
	</div>

	<div style="text-align: center;">
        <input class="botao" type="button" value="Fechar" onclick="window.close();">
    </div>
	
</body>
</html>




