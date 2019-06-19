<?php
/**
 * Classe de controle do  iniciativa_planejamento_detalheetapa
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   HEMERSON EXPEDITO DORNELAS MORAIS <hemerson.moraiss@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */

include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_detalheetapa.class.inc';
include_once APPRAIZ . 'par3/classes/model/Iniciativa_planejamento_detalheetapaHistorico.class.inc';

/**
 * Par3_Controller_Iniciativa_planejamento_detalheetapa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-07-2017
 * @link     no link
 */
class Par3_Controller_Iniciativa_planejamento_detalheetapa
{
    private $model;

    public function __construct()
    {
        $ipidid = $_POST['ipidid'];
        $this->model = new Par3_Model_Iniciativa_planejamento_detalheetapa($ipidid);
        $this->modelHistorico = new Par3_Model_Iniciativa_planejamento_detalheetapaHistorico();
    }


    public function formDetalheEtapa($arrPost)
    {

        global $simec;
        echo '<input type="hidden" name="inpid" value="' . $arrPost["inpid"] . '"/>';
        echo '<input type="hidden" name="ipidid" value="' . $this->model->ipidid . '"/>';
        echo '<div class="form-group"><label class="col-md-3 control-label">Código:</label>';
        echo '<div class="col-md-2"><input type="text" disabled="" value="' . $this->model->ipidid . '" class="form-control"></div>';
        echo '<div style="clear:both"></div>';
        echo '</div>';
        echo $simec->input('ipidatividade', 'Atividade', $this->model->ipidatividade, array('maxlength' => '255'));
        echo $simec->input('ipiddescricao', 'Descrição Atividade', $this->model->ipiddescricao, array('maxlength' => '255'));
        echo $simec->input('ipidunidade', 'Unidade', $this->model->ipidunidade, array('maxlength' => '255'));
        echo $simec->input('ipidquantidadehoras', 'Quantidade', $this->model->ipidquantidadehoras, array('maxlength' => '12'));
        echo $simec->input('ipidtotalhoras', 'Total  Horas Aulas', $this->model->ipidtotalhoras, array('maxlength' => '12'));
        echo '<script>
                                
                            $(function () {
                                $(\'#ipidquantidadehoras\').bind(\'keydown\', soNums); //
                                $(\'#ipidtotalhoras\').bind(\'keydown\', soNums); //
                        
                            });
                            function soNums(e) {
                        
                                //teclas adicionais permitidas (tab,delete,backspace,setas direita e esquerda)
                                keyCodesPermitidos = new Array(8, 9, 37, 39, 46);
                        
                                //numeros e 0 a 9 do teclado alfanumerico
                                for (x = 48; x <= 57; x++) {
                                    keyCodesPermitidos.push(x);
                                }
                       
                                //numeros e 0 a 9 do teclado numerico
                                for (x = 96; x <= 105; x++) {
                                    keyCodesPermitidos.push(x);
                                }
                        
                                //Pega a tecla digitada
                                keyCode = e.which;
                        
                                //Verifica se a tecla digitada é permitida
                                if ($.inArray(keyCode, keyCodesPermitidos) != -1) {
                                    return true;
                                }
                            return false;
                             }
</script>';

    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar($arrPost)
    {
        global $url;

        $this->model->popularDadosObjeto();
        //   $this->model->popularDadosObjeto();
        $url .= '&ipidid=' . $this->model->ipidid;


        $arrayDetalhe['ipidatividade'] = trim(($arrPost['ipidatividade']));
        $arrayDetalhe['ipiddescricao'] = trim(($arrPost['ipiddescricao']));
        $arrayDetalhe['ipidunidade'] = trim(($arrPost['ipidunidade']));
        $arrayDetalhe['ipidquantidadehoras'] = $arrPost['ipidquantidadehoras'];
        $arrayDetalhe['ipidtotalhoras'] = $arrPost['ipidtotalhoras'];


        try {
            $this->model->popularDadosObjeto($arrayDetalhe);
            $sucesso = $this->model->salvar();
            $this->model->commit();
            $this->modelHistorico->gravarHistorico($this->model, 'CREATE');
            $this->modelHistorico->commit();

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($sucesso) {
            return (int)$sucesso;
        }
        return 'erro';
    }

    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar($arrPost)
    {
        global $url;
        $id = $arrPost['ipidid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&ipidid={$id}";
        $iniciativa_planejamento_detalheetapa = new Par3_Model_Iniciativa_planejamento_detalheetapa($id);

        $atividadeEtapa = $this->verificaIntegridade($id);

        if ($atividadeEtapa > 0) {

            return 'erro';

        }

        try {
            $iniciativa_planejamento_detalheetapa->ipidstatus = 'I';
            $iniciativa_planejamento_detalheetapa->ipiddtinativacao = date('Y-m-d H:i:s');
            $iniciativa_planejamento_detalheetapa->ipidusucpfinativacao = $_SESSION['usucpf'];

            $sucesso = $iniciativa_planejamento_detalheetapa->salvar();
            $iniciativa_planejamento_detalheetapa->commit();

            $this->modelHistorico->gravarHistorico($iniciativa_planejamento_detalheetapa, 'DELETE');

        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
        if ($sucesso) {
            return (int)$sucesso;
        }
        return 'erro';

    }


    public function listaDetalhe($inpid, $buff = false)
    {
        $sql = "SELECT
                      ipid.ipidid ,
                      ipid.inpid ,
                      ipid.ipidatividade ,
                      ipid.ipiddescricao ,
                      ipid.ipidunidade ,
                      ipid.ipidquantidadehoras ,
                      ipid.ipidtotalhoras ,
                      ipid.ipidstatus 
                    
                      FROM PAR3.iniciativa_planejamento_detalheetapa ipid
                      INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = ipid.inpid
                      WHERE   ipid.inpid = $inpid AND ipid.ipidstatus = 'A'
                      ORDER BY ipid.ipidid";


        $cabecalho = array('Atividade', 'Descrição Etapa', 'Unidade', 'Quantidade', 'Total de Horas');
        $esconderColunas = array('ipidid', 'inpid', 'ipidstatus');
        if($buff){
           
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
        }else{
            
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO);
        }
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->esconderColunas($esconderColunas);
        $listagem->addAcao('remove', 'removerIDetalhesEtapa');
        $listagem->addAcao('edit', 'editarDetalhesEtapa');
       // $listagem->turnOnOrdenacao();
        return $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }
    

    public function verificaIntegridade($idDetalheEtapa)
    {
        global $db;
        $sql = "select * from par3.iniciativa_planejamento_atividadeetapa where ipidid = {$idDetalheEtapa} and ipiastatus = 'A'";
        $result = $db->executar($sql);
        return pg_num_rows($result);
    }
    
    public function verificaQntDetalhes($inpid)
    {
        global $db;
        
        $sql =  "SELECT
                      ipid.inpid
                    
                      FROM PAR3.iniciativa_planejamento_detalheetapa ipid
                      INNER JOIN par3.iniciativa_planejamento ip on ip.inpid = ipid.inpid
                      WHERE   ipid.inpid = $inpid AND ipid.ipidstatus = 'A'";
                     

        $result = $db->executar($sql);
        return pg_num_rows($result);
    }
}

?>