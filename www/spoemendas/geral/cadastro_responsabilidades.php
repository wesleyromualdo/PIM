<?php
/**
 * Consulta de responsábilidades atribuídas ao perfil de um usuário.
 * $Id: cadastro_responsabilidades.php 141386 2018-06-28 14:00:46Z victormachado $
 */
include "config.inc";
header('content-type: text/html; charset=utf-8');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/spoemendas/_constantes.php');
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
$db = new cls_banco();
$esquema = 'spoemendas';

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if (!$pflcod && !$usucpf) {
    ?><font color="red">Requisição inválida</font><?php
    exit();
}

$sqlResponsabilidadesPerfil = <<<DML
    SELECT tr.*
    FROM {$esquema}.tprperfil p
    INNER JOIN {$esquema}.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
    WHERE tprsnvisivelperfil = TRUE
        AND p.pflcod = '%s'
    ORDER BY tr.tprdsc
DML;
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);

$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil) < 1) {
    print "<font color='red'>Não foram encontrados registros</font>";
} else {
    foreach ($responsabilidadesPerfil as $rp) {
        //
        // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
        $sqlRespUsuario = "";
        switch ($rp["tprsigla"]) {
            case 'A': // Unidade Orçamentária
                $cabecalho = array('Código', 'Descrição');
                $aca_prg = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
                    SELECT uni.unicod AS codigo,
                        uni.unicod || ' - ' || uni.unidsc AS descricao
                    FROM {$esquema}.usuarioresponsabilidade ur
                    INNER JOIN public.unidade uni ON ur.unicod::integer = uni.unicod::integer
                    WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
DML;
                break;
            case 'B': // Parlamentar
                $cabecalho = array('Código', 'Descrição');
                $aca_prg = 'Parlamentar(es) associado (s)';
                $sqlRespUsuario = <<<DML
                    SELECT 
                        a.autcod as codigo,
                        a.autcod || ' - ' ||  a.autnome as descricao                     
                    FROM {$esquema}.usuarioresponsabilidade ur
                    INNER JOIN emenda.autor a ON (a.autid = ur.autid)
                    WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
DML;
                break;
	        case 'S'://Secretaria
		        $cabecalho = array('Código', 'Descrição');
		        $aca_prg = 'Secretaria(s) associado (s)';
		        $sqlRespUsuario = <<<DML
                    SELECT uni.secid AS codigo,
                           uni.secid || ' - ' || uni.seccod AS descricao
                      FROM {$esquema}.usuarioresponsabilidade ur
                        INNER JOIN public.secretaria uni ON uni.secid = ur.secid
                      WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
DML;
		        break;
            case 'D':
                $cabecalho = array('Código', 'Descrição');
                $aca_prg = 'Unidade(s) Gestora(s) associada(s)';
                $sqlRespUsuario = <<<DML
                    SELECT uni.ungcod AS codigo,
                           uni.ungcod || ' - ' || uni.ungdsc AS descricao
                      FROM {$esquema}.usuarioresponsabilidade ur
                        INNER JOIN public.unidadegestora uni ON uni.ungcod = ur.ungcod
                      WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
DML;
                break;
		        
        }
        if (!$sqlRespUsuario)
            continue;
        $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setQuery($query);
        $listagem->setCabecalho($cabecalho);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('descricao', function($valor){
            return <<<HTML
<p style="text-align:left!important">{$valor}</p>
HTML;
        });
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
$db->close();
exit();
