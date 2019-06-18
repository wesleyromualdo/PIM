<?php
/**
 * Classe de controle do PNE
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Pne
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */
class Par3_Controller_Pne extends Modelo
{
	public function __construct()
	{
		parent::__construct();
	}

    /**
     * Função desenharTela
     * - desenha a tela da unidade de acordo com o menu a ser preenchido
     *
     * @return escreve a tela.
     *
     */
    public function desenharTela($menu)
    {
        $pastaPne = 'par3/modulos/principal/planoTrabalho/pne/';

        $aba = 'possuiDiagnostico';

        if (0 < $menu && $menu < 21) $aba = 'meta';

        switch($aba){
        	case 'meta':
        	    include APPRAIZ.$pastaPne.'meta.php';
        	    break;
        	case 'possuiDiagnostico':
        	default:
        	    include APPRAIZ.$pastaPne.'possuiDiagnostico.php';
        	    break;
        }

    }//end desenharTela();


    /**
     * Função situacaoPne
     * - retorna situacao PNE
     *
     * @return escreve a tela.
     *
     */
    public function situacaoPne($inuid)
    {
        $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
        $controllerPne   = new Par3_Controller_Pne();

        if ($modelUnidade->itrid === '2') {
            require_once APPRAIZ.'sase/classes/Assessoramento.class.inc';
            $assessoramento = new Assessoramento();
            return $assessoramento->pegaDscSituacao($modelUnidade->muncod);
        } else {
            require_once APPRAIZ.'sase/classes/AssessoramentoEstado.class.inc';
            $assessoramento = new AssessoramentoEstado();
            return $assessoramento->pegaDscSituacao($modelUnidade->estuf);
        }
    }//end situacaoPne();


    /**
     * Função preenchimentoPossuiDiagnostico
     * - retorna icone de preenchimento do menu possui diagnostico
     *
     * @return string.
     *
     */
    public function preenchimentoPossuiDiagnostico($inuid)
    {
        $modelUnidade    = new Par3_Model_InstrumentoUnidade($inuid);
        $controllerPne   = new Par3_Controller_Pne();

        if ($modelUnidade->itrid === '2') {
            require_once APPRAIZ.'sase/classes/Assessoramento.class.inc';
            $assessoramento = new Assessoramento();
            $stacod         = $assessoramento->pegaStacod($modelUnidade->muncod);
        } else {
            require_once APPRAIZ.'sase/classes/AssessoramentoEstado.class.inc';
            $assessoramento = new AssessoramentoEstado();
            $stacod         = $assessoramento->pegaDscSituacao($modelUnidade->estuf);
        }

        $iconNaoPreenchido = '<span class="fa fa-circle" ></span>';
        $iconPreenchido    = '<span class="fa fa-check iconUnidade"'.
                                    'style="color:green !important;'.
                                    'background-color: transparent;"></span>';

        if ($stacod > 3) {
            return $iconPreenchido;
        } else {
            return $iconNaoPreenchido;
        }
    }//end preenchimentoPossuiDiagnostico();


    /**
     * Função retornaArrayMetas
     * - retorna array de metas
     *
     * @return array.
     *
     */
    public function retornaArrayMetas()
    {
        require_once APPRAIZ.'sase/classes/Metas.class.inc';
        $metas = new Metas();

        return $metas->retornaArrayListaGeral();
    }//end retornaArrayMetas();

    public function verificaPreenchimentoPNE($inuid, $detalhado = false)
    {
    	$obPendencia = new Par3_Helper_PendenciasEntidade();
    	require_once APPRAIZ.'sase/classes/Metas.class.inc';
        $obMeta = new Metas();

        $intExisteArquivo = $obPendencia->existeArquivoPNE($inuid);

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $estuf = $instrumentoUnidade->estuf;
        $muncod = $instrumentoUnidade->muncod;
        $itrid = $instrumentoUnidade->itrid;

        $indicadoresTotal = $obMeta->retornarNumeroIndicadores($estuf, $muncod , $itrid);
        $indicadoresPendentes = $obMeta->retornarIndicadoresPendentes($estuf, $muncod , $itrid);
        $indicadoresPendentesTotal = is_array($indicadoresPendentes) ? count($indicadoresPendentes) : 0;
        $indicadoresPreenchidos = $indicadoresTotal - $indicadoresPendentesTotal;

        $perc = $intExisteArquivo + $indicadoresPreenchidos;
        $perc = ( $perc * 100 ) / ( $indicadoresTotal + 1 );

        if ($detalhado) {
        	$retorno = array();

        	$retorno[] = array(
        		'descricao' => 'Arquivo PNE',
        		'percent' => $intExisteArquivo ? 100 : 0,
        		'total' => 1,
        		'url' => "par3.php?modulo=principal/planoTrabalho/pne&acao=A&inuid={$inuid}"
        	);

            if (is_array($indicadoresPendentes)) {
                foreach ($indicadoresPendentes as $indicador) {
                    $retorno[] = array(
                        'descricao' => $indicador['subtitulo'],
                        'percent' => ((int)$indicadoresPreenchidos * 100) / $indicadoresTotal,
                        'total' => $indicadoresTotal,
                        'url' => "par3.php?modulo=principal/planoTrabalho/pne&acao=A&inuid={$inuid}&metid={$indicador['metid']}"
                    );
                }
            }

        	return $retorno;
        } else {
	    	return (int) $perc;
        }
    }


    /**
     * Função camposComparacaoMetasPNE
     * - retorna array de metas
     *
     * @return array.
     *
     */
    public function camposComparacaoMetasPNE($arrPost)
    {
        global $simec;

        $sql = Territorios_Model_Regiao::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>Região</b></div>
                    <div class="panel-body">';
        echo $simec->select('regcod[]', '', $arrPost['regcod'], $sql, $arrAttr);
        echo '      </div>
                </div>';

        $sql = Territorios_Model_Estado::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>UF</b></div>
                    <div class="panel-body">';
        echo $simec->select('estuf[]', '', $arrPost['estuf'], $sql, $arrAttr);
        echo '      </div>
                </div>';

        $sql = Territorios_Model_Mesoregiao::pegarSQLSelect($arrPost);
        echo '  <div class="panel panel-default">
                    <div class="panel-heading"><b>Mesoregião</b></div>
                    <div class="panel-body">';
        echo $simec->select('mescod[]', '', $arrPost['mescod'], $sql, $arrAttr);
        echo '      </div>
                </div>';
    }//end camposComparacaoMetasPNE();

}//end Class
?>