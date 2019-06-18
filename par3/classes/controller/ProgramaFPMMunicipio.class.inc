<?php
/**
 * Classe de controle do  programa_fpm_municipio
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   LEO KENZLEY BESERRA DE OLIVEIRA <leokenzley@gmail.com>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-07-2018
 * @link     no link
 */



/**
 * Par3_Controller_Programa_fpm_municipio
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 11-07-2018
 * @link     no link
 */
class Par3_Controller_ProgramaFPMMunicipio
{
    private $model;
    private $modelProadesao;
    private $modelPrograma;
    private $modelAdesaoPrograma;
    private $modelWorkflowDocumento;
    private $modelProgramaFPMMunicipioEscola;
    private $modelProgramaFPMMunicipio;

    public function __construct()
    {

        $this->model = new Par3_Model_ProgramaFPMMunicipio();
        $this->modelProadesao = new Par3_Model_Proadesao();
        $this->modelPrograma = new Par3_Model_Programa();
        $this->modelAdesaoPrograma = new Par3_Model_AdesaoPrograma();
        $this->modelWorkflowDocumento = new Workflow_Model_Documento();
        $this->modelProgramaFPMMunicipioEscola = new Par3_Model_ProgramaFPMMunicipioEscola();
        $this->modelProgramaFPMMunicipio = new Par3_Model_ProgramaFPMMunicipio();

    }
    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Par3_Controller_Programa_fpm_municipio();
        $this->model->popularDadosObjeto();
        $url .= '&fpmid=' . $this->model->fpmid;

        try{
            $sucesso = $this->model->salvar();
            $this->model->commit();
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }

        if($sucesso){
            simec_redirecionar($url, 'success');
        }
        simec_redirecionar($url, 'error');
    }


    /**
     * Função excluir
     * - grava os dados
     *
     */
    public function inativar()
    {
        global $url;
        $id = $_GET['fpmid'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&fpmid={$id}";
        $programa_fpm_municipio = new Par3_Model_Programa_fpm_municipio($id);
        try{
            $programa_fpm_municipio->Xstatus = 'I';
            $programa_fpm_municipio->Xdtinativacao = date('Y-m-d H:i:s');
            $programa_fpm_municipio->Xusucpfinativacao = $_SESSION['usucpf'];

            $programa_fpm_municipio->salvar();
            $programa_fpm_municipio->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inuid
     * @description Este método carrega o valor para calculo para o campo Total de matrículas de 3º ao 9ª ano da Rede,
     * que é a soma de todas as matrículas feitas nas escolas municipais
     */
    public function getTotalMatricula3a9DaRedeByInuid($inuid){
        if(!empty($inuid)){
            return $this->model->getTotalMatricula3a9DaRedeByInuid($inuid);
        }else{
            return null;
        }
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $dados
     * @throws Exception
     * @description Atualiza o pfasobre da tabela par3.proadesao
     */
    public function updateSobre($dados){
        return $this->modelProadesao->updateSobre($dados);
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $prg
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description Este método pega todos os campos da tabela par3.programa e par3.proadesao pelo id do programa
     */
    public function fetchAllProgramaById($prg){
        return $this->modelPrograma->fetchAllProgramaById($prg);
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inuid
     * @param $prgid
     * @description verifica se já existe um fluxo
     */
    public function isFluxoIniciado($prgid,$inuid){
        return $this->model->isFluxoIniciado($prgid,$inuid);
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $docidInserido
     * @param $prgid
     * @param $inuid
     * @throws Exception
     * @description iniciar o fluxo
     */
    public function iniciaFluxo($docidInserido,$inuid){
        $this->model->iniciaFluxo($docidInserido,$inuid);
    }



    public function getProgramaFPMByIdUnidade($inuid){
        return $this->model->fetchProgramaFPMByIdUnidade($inuid);
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inuid
     * @return bool
     * @throws Exception
     * @description retorna resumo da adesão da unidade
     */
    public function getDadosDaAdesaoByIdInstrumentoUnidade($inuid){
        return $this->modelAdesaoPrograma->fetchDadosDaAdesaoByIdInstrumentoUnidade($inuid);
    }

    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inuid
     * @return array
     * @description Este método retorna os valores da carga do programa novo mais educação ligados a unidade
     */
    public function getDadosCargaProgramaNovoMaisEducacao($inuid){
        return array();
    }


    /**
     * @param $inuid
     * @param $vlestudanteporescola
     */
    public function salvaDadosPorEscola($dados){
       return $this->modelProgramaFPMMunicipioEscola->salvaDadosEscolas($dados);
    }

    public function getDadosMunicipioByUnidade($inuid){
        return $this->modelProgramaFPMMunicipio->getDadosMunicipioByUnidade($inuid);
    }

    /**
     * @param $fmpid
     * @param $dados
     */
    public function salvaDadosMunicipio($dados){
        return $this->modelProgramaFPMMunicipio->salvaDadosMunicipio($dados);
    }

    public function getEsddesc($inuid){
       return $this->model->getEstado($inuid);
    }

    public function getDadosDaUnidade($inuid){
        return $this->model->getDadosDaUnidade($inuid);
    }


    /**
     * @author Leo Kenzley <leokenzley@gmail.com>
     * @param $inuid
     * @return array|bool|mixed|NULL
     * @throws Exception
     * @description carrega os valores dos campos
     *
     */
    public function carregaValueCamposMatriculas($inuid){
        return $this->model->carregaValueCamposMatriculas($inuid);
    }

    public function cerregaValoresPorEscola($inuid){
        return $this->model->carregaDadosEscolasDaUnidade($inuid);
    }

    public function cerregaValoresPorEscolaJaComValores($inuid){
        return $this->model->carregaDadosEscolasDaUnidadeJaComValores($inuid);
    }

    public function getDadosFnde2018ByUnidade($inuid){
        return $this->model->getDadosFnde2018ByUnidade($inuid);
    }

    public function updateEstadoDocumento($inuid,$docid){
        $this->model->updateEstadoDocumento($inuid,$docid);
    }

    /**
     * @param $inuid
     * @param $pfaid
     * @description Este método pega os valores do workflow Estado e Documento
     */
    public function getDadosWorkFlowUnidadePrograma($inuid,$pfaid){
        return $this->modelAdesaoPrograma->getDadosWorkFlowUnidadePrograma($inuid,$pfaid);
    }

    public function getSomaVlJaCadastradosNasEscolas($inuid){
        return $this->modelProgramaFPMMunicipioEscola->getSomatoriaValoresEscolasPorEntidade($inuid);
    }

    /**
     * @param $dados
     * @throws Exception
     * @Erros
     *  E1 = o valor do critério 1 é maior que A - (B-C)
     */
    public function validaValorMunicipioEscolas($dados){
        //MUNICIPIO
        $arrayErros = array();
        $isValid = true;
        $inuid = $dados['inuid'];
        $dadosValorFndeDoMunicipio = $this->model->getDadosFnde2018ByUnidade($inuid);
        $valorCamposMatricula = $this->model->carregaValueCamposMatriculas($inuid);
        $a = $valorCamposMatricula['total_matricula_ensino_fundamental'];
        $b = $valorCamposMatricula['total_matriculas_atendidas_pdde_integral'];
        $c = $valorCamposMatricula['total_matricula_tempo_integral_censo'];
        $e = $dadosValorFndeDoMunicipio['vl_repassefnde2018'];
        $f = $dados['fpmvlaplicadopnmemunicipio'];
        $d = ($a - ($b + $c));
        $criterio1 = (int) $dados['fpmqtmatriculaaptasatendimentocriterio1'];
        //trata valor a ser aplicado ao municipio
        $vl_a_ser_aplicado = $dados['fpmvlaplicadopnmemunicipio'];
        $vl_a_ser_aplicado = str_replace(".","",$vl_a_ser_aplicado); //tira pontos
        $vl_a_ser_aplicado = str_replace(",",".",$vl_a_ser_aplicado); //tira pontos

        $g = (!empty($f)) ? ($f / 120) : 0;
        $g_arredondado = ceil($g);
        if($criterio1 == 0 || empty($criterio1)){
            $arrayErros[] = "O valor do campo 'Matrículas aptas para atendimento ? critério 1' não pode ser vazio ou 0";
        }

        if($criterio1 > $d){
            $arrayErros[] = "O valor do campo 'Matrículas aptas para atendimento ? critério 1' não pode ser maior que {$d}";
        }

        if($d == 0){
            $arrayErros[] = "O valor do campo 'Matrículas aptas para atendimento ? critério 1' não pode ser 0";
        }

        if(empty($vl_a_ser_aplicado) || $vl_a_ser_aplicado == 0 || $vl_a_ser_aplicado == 0.0){
            $arrayErros[] = "O valor do campo 'Valor a ser aplicado no PNME pelo município' não pode ser maior que 'Valor repassado pela Resolução FNDE nº 11/2018' e menor que 120,00";
        }

        if($vl_a_ser_aplicado > $e){
            $arrayErros[] = "O valor do campo 'Valor a ser aplicado no PNME pelo município' não pode ser maior que 'Valor repassado pela Resolução FNDE nº 11/2018'";
        }

      /*  $menor_valor = ($dados['fpmvlaplicadopnmemunicipio'] > $dados['fpmqtmatriculaaptasatendimentocriterio1']) ? $dados['fpmqtmatriculaaptasatendimentocriterio1'] : $g_arredondado;
        ver($dados); exit();
        //ESCOLA
        $vl_soma_escolas = $this->getSomaEnviadoEscola($dados['vlestudanteporescola']); //pega o valor digitados das escolas
       // ver($vl_soma_escolas,$menor_valor);exit();
        if($vl_soma_escolas != $menor_valor){
            $arrayErros[] = "A soma de 'Estudantes para atendimento no PNME 15h' deve ser igual a {$menor_valor}";

        }*/

        $arrayResult = array();
        if(count($arrayErros) != 0){
            $arrayResult = array(
                'erro' => true,
                'msg_error' => $arrayErros,
            );
        }else{
            $arrayResult = array(
                'erro' => false,
                'msg_error' => "",
            );
        }

    return $arrayResult;
    }

    /**
     * @param $vlestudanteporescola
     * @return int
     *
     */
    private function getSomaEnviadoEscola($vlestudanteporescola){
        $vl = 0;
        if(count($vlestudanteporescola) > 0){
            foreach($vlestudanteporescola as $d){
                $arrayValues = explode("|",$d);
                $vl = ($vl + ((int) $arrayValues[1]));
            }
        }
        return $vl;
    }

    public function getTomaTotalMatriculaAtendida($inuid){
        return $this->modelProgramaFPMMunicipioEscola->getSomatoriaValoresEscolasPorEntidade($inuid);
    }

    public function getMunicipioExport(){
        return $this->model->getMunicipioExport();
    }

}
?>