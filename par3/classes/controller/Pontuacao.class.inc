<?php
/**
 * Classe de controle da Pontuação
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */

/**
 * Par3_Controller_Pontuacao
 *
 * @category Class
 * @package  A1
 * @author   Victor Benzi <victorbenzi@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 20-10-2015
 * @link     no link
 */
class Par3_Controller_Pontuacao extends Modelo
{
    public function __construct()
    {
        parent::__construct();
    }

    /**
     * Função recuperaPontuacao
     * - Recupera os critérios pontuados no indicador
     *
     */
    public function recuperaPontuacao($indid, $inuid){
        $oPontuacaoModel = new Par3_Model_Pontuacao();
        $pontuacao = $oPontuacaoModel->buscaPontuacao($inuid, $indid);
        $ptoid = $pontuacao['ptoid'] ? $pontuacao['ptoid'] : null;

        if( $ptoid ){
            $oPontuacaoCriterio = new Par3_Model_PontuacaoCriterio();
            $criterios = $oPontuacaoCriterio->recuperarDados($ptoid);
            if( is_array($criterios) && $criterios[0] ){
                foreach( $criterios as $crt ){
                    $pontuacao['criterios'][$ptoid][$crt['crtid']]['crtid'] = $crt['crtid']; //array_column($criterios,('ptcpontuacao' ? 'ptcpontuacao' : 'crtid'),'crtid');
                    if( $crt['ptcpontuacao'] ){
                        $pontuacao['criterios'][$ptoid][$crt['crtid']]['pontuacao'] = $crt['ptcpontuacao'];
                    }
                }
            }
        }

        return $pontuacao;
    }


    /**
     * Função preparaSalvar
     * - Salva os critérios do instrumento
     *
     */
    public function preparaSalvar()
    {
        $inuid = $_POST['inuid'];
        $dimid = $_POST['dimid'];
        $areid = $_POST['areid'];
        $indid = $_POST['indid'];
// ver($_POST, d);
        if( is_array($_POST['indicador'][$dimid][$areid]) ){
            $arr = array_keys($_POST['indicador'][$dimid][$areid]);
            $arr = is_array($arr) ? $arr : array();
            if( $indid != $arr[0] && $arr[0] != '' ){
                $indid = $arr[0];
            }
        }

        if(!$indid){
            simec_redirecionar('par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid='.$_POST['inuid'], 'error');
        }

        $ptojustificativa = $_POST['indjustificativa'][$indid];

        // Busca se a pontuação já foi salva
        $oPontuacao = new Par3_Model_Pontuacao;
        $pontuacao = $oPontuacao->buscaPontuacao($inuid, $indid);
        $ptoid = $pontuacao['ptoid'] ? $pontuacao['ptoid'] : null;

        // Salvar Pontuacao
        $ptoid = $oPontuacao->preparaSalvar($ptoid, $inuid, $ptopontuacao, $_SESSION['usucpf'], $ptojustificativa, $indid);

        // Exclui os critérios já cadastrados
        $oPontuacaoCriterio = new Par3_Model_PontuacaoCriterio;
        $criterios = $oPontuacaoCriterio->recuperarDados($ptoid);
        if( is_array($criterios) ){
            foreach( $criterios as $criterio ){
                $oPontuacaoCriterio->preparaExcluir($criterio['ptcid']);
            }
        }

        // Salvar os critérios de resposta
        if( is_array($_POST['indicador'][$dimid][$areid][$indid]) ){
            foreach( $_POST['indicador'][$dimid][$areid][$indid] as $criterioSalvar ){
                $ptcpontuacao = $_POST['ptcpontuacao'][$criterioSalvar] ? $_POST['ptcpontuacao'][$criterioSalvar] : null;
                $oPontuacaoCriterio = new Par3_Model_PontuacaoCriterio;
                $oPontuacaoCriterio->preparaSalvar($ptoid, $criterioSalvar, $ptcpontuacao);
            }
        }

        $oPontuacaoMeta = new Par3_Model_PontuacaoMeta();

        // Exclui as metas já salvas
        $pontuacaoMetas = $oPontuacaoMeta->recuperarDados($ptoid);
        if( is_array($pontuacaoMetas) ){
            foreach( $pontuacaoMetas as $metaSalva ){
                $oPontuacaoMeta->preparaExcluir($metaSalva['pmtid']);
            }
        }

        // Salva as metas da pontuação
        if( is_array($_POST['metas'][$indid])){
             $metas = $_POST['metas'][$indid];
            if( is_array($metas) ){
                // Salva as novas metas
                foreach( $metas as $mt ){
                    if($ptoid && $mt){
                        $oPontuacaoMeta->preparaSalvar($ptoid, $mt);
                    }
                }
            }
        }


        return $indid;

    }//end preparaSalvar()


    public function filtroAjax($filtro,$nivel)
    {
        $oPainelModel = new Painel_Model_Seriehistorica();



        if(empty($filtro)){
            $escolas = $oPainelModel->tabelaEscolas();

        }else{
            $escolas = $oPainelModel->filtroTabela($filtro);

        }


        if ($nivel == 2) {

        ?>
        <div class="row" style="margin-top: 40px;">
            <div class="col-md-12">

                <div class="ibox float-e-margins" style="margin: 25px;">
                    <div class="ibox-content">

                        <table class="table table-hover dataTable">
                            <thead>
                            <tr>
                                <th>Escola</th>
                                <th>Equipamento</th>
                                <th>Quantidade existente do equipamento</th>
                                <th>Demanda do equipamento</th>
                            </tr>
                            </thead>

                            <?php
                            foreach ($escolas as $key) {
                                ?>
                                <tr>
                                    <td><?php echo $key['escola'] ?></td>
                                    <td><?php echo $key['tipo'] ?></td>
                                    <td><?php echo $key['totalequipamento'] ?></td>
                                    <td><?php echo $key['totaldemanda'] ?></td>
                                </tr>
                            <?php } ?>
                        </table>
                    </div>
                </div>

            </div>
        </div>
    <?php
    }
    else{

        $IndicadorSimec = new Simec_Indicador();
         $IndicadorSimec->gerarTabela(3205,3275);


}
    }



}//end Class
?>