<?php
date_default_timezone_set('America/Sao_Paulo');
$data = date('Y-m-d');

if(($data >= '2017-11-14') && ($data <= '2017-11-19') ){
?>
<script>
    alert('Em manutenção até 19/11/2017. Em caso de dúvidas, entrar em contato pelo e-mail dase@mec.gov.br');
    window.location = '/';
</script>
<?php
 die;
}
ob_start();



// --------------- Dependências
include_once '../../sase/classes/Mapa/MetaDados.class.inc';
include_once '../../sase/classes/Mapa/Poligonos.class.inc';
include_once '../../sase/classes/Mapa/Mapas.class.inc';
include_once '../../sase/classes/Assessoramento.class.inc';
include_once '../../sase/classes/SituacaoAssessoramento.class.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';



include APPRAIZ . 'includes/cabecalho.inc';


$perfis = pegaPerfilGeral( $_SESSION['usucpf'] );



// --

if( array_search(PFLCOD_SASE_CONSULTA, $perfis) !== false
    && array_search(PFLCOD_SASE_SUPER_USUARIO, $perfis) === false )
    $pagina = 'mapa';

// --------------- Cabeçalho
$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino.";
$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação.";
$titulo = 'Diagnóstico  Município';
//$titulo = 'Assistência Técnica';

?>
    <link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>

    <div id='cabecalhoSistema' style="margin-bottom: 20px">
        <?='<h6><small>'.implode('</small></h6><h6><small>', $cabecalhoSistema).'</small></h6>'?>
        <center><h3><?=$titulo?></h3></center>
    </div>

<?php
// --

// --------------- Inclusão da Página
include APPRAIZ . "sase/modulos/principal/sistemasdeensino/diagnostico/listamunicipio.inc";