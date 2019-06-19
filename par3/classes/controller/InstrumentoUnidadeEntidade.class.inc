<?php
/**
 * Classe de controle do InstrumentoUnidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

/**
 * Par3_Controller_InstrumentoUnidadeEntidade
 *
 * @category Class
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 29-09-2015
 * @link     no link
 */

include_once APPRAIZ . "par3/classes/model/InstrumentoUnidade.class.inc";

class Par3_Controller_InstrumentoUnidadeEntidade extends Controle
{

    private $tenIdsRotina = array(
        '02' => TENID_PREFEITO, /*PREFEITO*/
        '10' => TENID_SECRETARIO_ESTADUAL, /*SECRETARIO ESTADUAL*/
        '28' => TENID_DIRIGENTE /*DIRIGENTE MUNICIPAL*/
    );

    public function __construct()
    {
        parent::__construct();
    }

    /**
     *
     * */
    public function enviaEmailDirigenteRotina( $arrEntidade )
    {
        /**CARREGA VARIÁVEIS*/
        $dadosUsu 	= $arrEntidade['login'];
        $novoUsu 	= ($dadosUsu['ususenha'] != '') ? true : false;
        $enviaEmail = (! $dadosUsu['error'] ) ? true : false;
        $codCargo	= $arrEntidade['co_tp_entidade'];


        /*** CASO NÃO HAJA ERRO ENVIA O EMAIL*/
        if( $enviaEmail )
        {
            $arrCargo = array(
                '02' => 'Prefeito',
                '28' => 'Dirigente Municipal',
                '10' => 'Secretário Estadual'
            );

            if( ($codCargo == '02') || ($codCargo == '28') )
            {
                $objMun = new Par3_Model_Municipio();

                $municipio = $objMun->descricaoMunicipio($arrEntidade['muncod'], TRUE);
                $tipoEsfera = 'Município';
                $unidade = $municipio;
                $strAssunto =  "Cadastro {$arrCargo[$codCargo]} do Município  " . $municipio ;

            }
            else if($codCargo == '10')
            {

                $objEst = new Par3_Model_Estado();

                $estado = $objEst->descricaoEstado($arrEntidade['estuf']);
                $tipoEsfera = 'Estado';
                $unidade = $estado;
                $strAssunto =  "Cadastro {$arrCargo[$codCargo]} do Estado  " . $estado ;
            }

            $strAssunto =  "Acesso ao SIMEC  Módulo PAR - " . $strAssunto;

            // CASO SEJA UM NOVO USUÁRIO
            if($novoUsu)
            {

                //>
                $strMensagem =
                    "
                <pre align=\"center\" style=\"text-align: justify;\"  >

                Prezado(a) Sr(a). {$arrEntidade['entnome']},

                Foi concluído com sucesso seu cadastramento no Sistema SIMEC-PAR.

                O acesso a este sistema é concedido em função da sua habilitação como $arrCargo[$codCargo] no {$tipoEsfera} de {$unidade}, concluída com sucesso junto ao Ministério da Educação  MEC e Fundo Nacional de Desenvolvimento da Educação  FNDE.

                Para realiza-lo, você deve acessar a página http://simec.mec.gov.br, e digitar seu CPF e a senha provisória informada abaixo.

                Usuário: {$dadosUsu['usucpf']}
                Senha provisória :{$dadosUsu['ususenha']}

                A senha provisória é gerada automaticamente pelo sistema, e acessível somente pelo endereço de email cadastrado. Ela será utilizada uma única vez e deverá ser substituída por uma senha pessoal no primeiro acesso ao sistema.

                Esta é uma mensagem automática e não é necessária nenhuma resposta ou confirmação.
                Em caso de dúvidas, favor entrar em contato pelo PAR Fale Conosco, disponível no link https://www.fnde.gov.br/parfaleconosco/index.php/publico

                <b> Ministério da Educação </b>

                </pre>";
            }
            else
            {
                $strMensagem =
                    "
                <pre align=\"center\" style=\"text-align: justify;\"  >

                Prezado(a) Sr(a). {$arrEntidade['entnome']},

                Foi concluído com sucesso seu cadastramento no Sistema SIMEC-PAR.

                O acesso a este sistema é concedido em função da sua habilitação como $arrCargo[$codCargo] no {$tipoEsfera} de {$unidade}, concluída com sucesso junto ao Ministério da Educação  MEC e Fundo Nacional de Desenvolvimento da Educação  FNDE.

                Para realiza-lo, você deve acessar a página http://simec.mec.gov.br, e digitar seu CPF e a senha.

Esta é uma mensagem automática e não é necessária nenhuma resposta ou confirmação.
    Em caso de dúvidas, favor entrar em contato pelo PAR Fale Conosco, disponível no link https://www.fnde.gov.br/parfaleconosco/index.php/publico

<b> Ministério da Educação </b>

    </pre>";

            }
            $strEmailTo = $arrEntidade['entemail'];

            $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $strMensagem = html_entity_decode($strMensagem);


            if($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br")
            {
                // Conforme solicitação da manuelita, enviar para o email real no simec-d
                $strEmailTo = array('elias.oliveira@mec.gov.br' );
                $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

            }
            else if($_SERVER['HTTP_HOST'] == "simec-local" )
            {
               
                $retorno = true;

            }
            else
            {
                $strEmailTo = $arrEntidade['entemail'];

                if( $strEmailTo )
                {
                    $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
                }
            }
        }
        return $retorno;
    }

    /**
     *
     * */
    public function atualizaDadosDirigente( $arrEntidade )
    {
        $modelSAPEDirigente     = new Par3_Model_SAPEDirigente();
        /** BUSCA OS DADOS DO DIRIGENTE DA ENTIDADE*/
        $dadosEntidadePar = $this->buscaDadosDirigenteRotina( $arrEntidade );
        $arrDadosEntidadePar 	= ( is_array($dadosEntidadePar['dadosEntidade']) ) ? $dadosEntidadePar['dadosEntidade'] : Array();

        /**VERIFICA SE É O MESMO VIGENTE OU SE SERÁ SUBSTITUIDO, ONDE O RETORNO É S = Substitui e A = atualiza */
        $acaoAtual = $this->comparaDirigenteVigente( $arrEntidade, $dadosEntidadePar );

        /** OBJ NECESSÁRIO PORQUE NÃO ESTÁ REFERENCIANDO PELO $THIS->MODEL */
        $objInuEnt = new Par3_Model_InstrumentoUnidadeEntidade();

        /** MONTA PARAMETROS PARA ATUALIZAR A ENTIDADE */
        $arrParams =
            array
            (
                'inuid'				=> $dadosEntidadePar['inuid'],
                'tenid'				=> $dadosEntidadePar['tenId'],
                'arrNovaEntidade' 	=> $arrEntidade,
                'arrEntidadeAntiga' => $arrDadosEntidadePar
            );

        /**ATUALIZA*/
        if($acaoAtual == 'A')
        {
            $idLogRotina = 3;
            $att = $objInuEnt->attDadosDirigentesEntidadeRotina($arrParams);
        }/**SUBSTITUI*/
        else
        {
            $idLogRotina = 5;
            $att = $objInuEnt->insereNovoDirigentesEntidadeRotina($arrParams);
        }
        /**EXECUTA AS MODIFICAÇÕES E RETORNA O ENTID*/
        if( $att )
        {
            if( $objInuEnt->commit() )
            {
                $modelSAPEDirigente->insereLogSucesso($arrEntidade, $idLogRotina);
                return $att;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }

    }
    /**
     *
     * */
    public function buscaDadosDirigenteRotina( $arrEntidade )
    {
        /** CARREGA O ID DA CARGA DO FNDE */
        $id = $arrEntidade['co_tp_entidade'];

        /** INSTANCIA INTRUMENTO UNIDADE */
        $objInu = new Par3_Model_InstrumentoUnidade();
        $objInuEnt = new Par3_Model_InstrumentoUnidadeEntidade();

        /**BUSCA O INUID*/
        if( ($id == '02') || ($id == '28'))
        {
            $inuid = $objInu->verificaInuidMunicipio($arrEntidade['muncod']);
        }
        else if($id == '10')
        {
            $inuid = $objInu->verificaInuidEstado($arrEntidade['estuf']);
        }

        /** BUSCA O DUTID (ID DO CARGO)*/
        $tenId	= $this->tenIdsRotina[$id];

        /** BUSCA OS DADOS DO CARGO DA ENTIDADE */
        $dadosEntidadePar = $objInuEnt->getDadosEntidade( $inuid, $tenId );

        $retorno['dadosEntidade']	= $dadosEntidadePar;
        $retorno['inuid']			= $inuid;
        $retorno['tenId']			= $tenId;

        return $retorno;

    }

    /**
     *
     * */
    public function comparaDirigenteVigente( $arrEntidade, $arrEntidadeAtual )
    {
        $cpfCarga = $arrEntidade['entcpf'];
        $cpfAtual = $arrEntidadeAtual['dadosEntidade']['entcpf'];

        if( (count($arrEntidadeAtual) > 0) && ($cpfCarga) && ($cpfAtual) )
        {
            if($cpfCarga == $cpfAtual)
            {
                return 'A';
            }
            else
            {
                return 'S';
            }
        }
        else
        {
            return 'S';
        }
    }

    public function verificaSituacaoPrefeitos($arrEntidade, $idTipo)
    {

        // Instancia intrumento unidade
        $objInu = new Par3_Model_InstrumentoUnidade();
        $objInuEnt = new Par3_Model_InstrumentoUnidadeEntidade();
        // Instancia dados da unidade
        // $oDadosUnidade = new DadosUnidade();

        $att = 0;
        $igual = 0;
        // Verifica as entidades
        foreach($arrEntidade as $k => $registro)
        {
            // Busca o inuid do par
            if( ($idTipo == '02') || ($idTipo == '28'))
            {
                $inuid = $objInu->verificaInuidMunicipio($registro['muncod']);
            }
            else if($idTipo == '10')
            {
                $inuid = $objInu->verificaInuidEstado($registro['estuf']);
            }

            //@trazer só o necessário

            $tenIds = array(
                '02' => TENID_PREFEITO, /*PREFEITO*/
                '10' => TENID_SECRETARIO_ESTADUAL, /*SECRETARIO ESTADUAL*/
                '28' => TENID_DIRIGENTE
            );

            $dadosEntidadePar = $objInuEnt->getDadosEntidade( $inuid, $tenIds[$idTipo] );


            $cpfPar 		= $dadosEntidadePar['entcpf'];
            $cpfAtual 		= $registro['entcpf'];

            $arrParams =
                array
                (
                    'inuid' 			=> $inuid,
                    'tenid' 			=> $tenIds[$idTipo],
                    'arrNovaEntidade' 	=> $registro,
                    'arrEntidadeAntiga' => $dadosEntidadePar
                );

            // Verifica se já está atualizado
            if( ($cpfPar != $cpfAtual) || $dadosEntidadePar[entstatus] == 'I' )
            {
                $att++;

                $objInuEnt->atualizaEntidadeRotina($arrParams);

            }
            else if($cpfPar == $cpfAtual)
            {
                $objInuEnt->atualizaInfomacoesDirigenteRotina($arrParams);
                $igual++;

            }
        }
        //getMailByMunicipioUf
    }


    /**
     * Função salvarEntidade
     * - salvar informações de PJ.
     *  - se o cnpj for diferente do já salvo ele inativa a atual e insere uma
     * PJ nova.
     *  - para diferenciar
     * @return void.
     *
     */
    public function salvarEntidade($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu={$arrDados['menu']}";

        try {

            if ($arrDados['entcpf_old'] != $arrDados['entcpf']) {
                unset($arrDados['entid']);
            }

            $endereco    = new Par3_Model_Endereco($arrDados['endid']);
            $entidade    = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
            $ipoEntidade = new Par3_Model_TipoEntidade($arrDados['tenid']);

            if ($arrDados['entcnpj'] != '') {
                $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')'), '', $arrDados['enttelefonecomercial']);
                $arrDados['entcnpj'] = str_replace(array('.', '-', '/'), '', $arrDados['entcnpj']);

                if ($entidade->entcnpj != $arrDados['entcnpj'] && $ipoEntidade->tenunico == 't') {
                    $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
                    $entidade->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                    unset($arrDados['entid']);
                    $entidade = new Par3_Model_InstrumentoUnidadeEntidade();
                }
            } else {
                $arrDados['entcpf'] = str_replace(array('.', '-', '/'), '', $arrDados['entcpf']);
                $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')', ' ', '_'), '', $arrDados['enttelefonecomercial']);
                $arrDados['enttelefonecelular'] = str_replace(array('(', '-', ')', ' ', '_'), '', $arrDados['enttelefonecelular']);
                $arrDados['enttelefonefax'] = str_replace(array('(', '-', ')', ' ', '_'), '', $arrDados['enttelefonefax']);

                if ($entidade->entcpf != $arrDados['entcpf'] && $ipoEntidade->tenunico == 't') {
                    $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
                    $entidade->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                    unset($_POST['entid']);
                    $entidade = new Par3_Model_InstrumentoUnidadeEntidade();
                }
            }

            if ($arrDados['endcep'] != '') {
                if ($endereco->endcep != $arrDados['endcep']) {
                    unset($arrDados['endid']);
                    $endereco = new Par3_Controller_Endereco();
                }

                $arrDados['endid'] = $endereco->salvarEndereco($arrDados);
            }

            $entidade->popularDadosObjeto($arrDados);
//             ver($entidade, $arrDados, d);
            $entidade->entstatus = 'A';
            $entidade->salvar();

            if ($arrDados['endcep'] != '') {
                $endereco = new Par3_Model_Endereco($arrDados['endid']);
                $endereco->entid = $entidade->entid;
                $endereco->salvar();
            }

            $entidade->commit();

            if (!$arrDados['booNaoRedireciona']) simec_redirecionar($url, 'success');

        } catch (Exception $e) {
            $entidade->rollback();
            if (!$arrDados['booNaoRedireciona']) simec_redirecionar($url, 'error', $e);

        }
    }//end salvarEntidade()


    /**
     * Função inativaEntidade
     * - inativa a entidade
     *
     * @param $_POST
     *
     * @return void.
     *
     */
    public function inativaEntidade($arrPost, $redirecionar = true)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A"
            . "&inuid={$arrPost['inuid']}&menu={$arrPost['menu']}";

        try {
            $entidade = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);

            $entidade->entstatus = 'I';

            $entidade->salvar();
            $entidade->commit();

            if($redirecionar){
                simec_redirecionar($url, 'success');
            }

        } catch (Exception $e) {
            $Secretaria->rollback();
            if($redirecionar){
                simec_redirecionar($url, 'error');
            }

        }
    }//end inativaEntidade()


    /**
     * Função verificaPreencimentoPrefeitura
     * - verifica o preenchimento dos dados da prefeitura e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoPrefeitura($inuid)
    {
        $prefeitura = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA;

        $objPrefeitura = $prefeitura->carregarDadosEntidPorTipo($inuid, $tenid);

        $objEndereco = new Par3_Model_Endereco($objPrefeitura->endid);


        $perc = 0;

        if ($objPrefeitura->entcnpj != '') $perc++;
        if ($objPrefeitura->entnome != '') $perc++;
        if ($objPrefeitura->entrazaosocial != '') $perc++;
        if ($objPrefeitura->entemail != '') $perc++;
        if ($objPrefeitura->enttelefonecomercial != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        /* A pedido da demanda 362444 if ($objEndereco->endnumero != '') $perc++;*/
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc / 8 * 100;

        return (int) $perc;

    }//end verificaPreencimentoPrefeitura();


    /**
     * Função verificaPreencimentoPrefeito
     * - verifica o preenchimento dos dados do prefeito e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoPrefeito($inuid)
    {
        $prefeito = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::PREFEITO;

        $objPrefeito = $prefeito->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco = new Par3_Model_Endereco($objPrefeito->endid);
        $perc = 0;

        if ($objPrefeito->entcpf != '') $perc++;
        if ($objPrefeito->entnome != '') $perc++;
        if ($objPrefeito->entemail != '') $perc++;
        if ($objPrefeito->enttelefonecomercial != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        /* A pedido da demanda 362444 if ($objEndereco->endnumero != '') $perc++;*/
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc / 7 * 100;

        return (int)$perc;

    }//end verificaPreencimentoPrefeito();


    /**
     * Função salvarInformacoesSecretaria
     * - salvar informações da Secretaria da unidade.
     *  - se o cnpj for diferente do já salvo ele inativa a atual e insere uma
     * Secretaria nova.
     *
     * @return void.
     *
     */
    public function salvarInformacoesSecretaria($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A"
            . "&inuid={$arrDados['inuid']}&menu=secretaria";

        $endereco = new Par3_Model_Endereco($arrDados['endid']);
        $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

        try {
            $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')'), '', $arrDados['enttelefonecomercial']);

            if ($Secretaria->entnome != $arrDados['entnome'] && $arrDados['entnome'] != '') {
                $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
                $Secretaria->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($arrDados['entid']);
                $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            if ($endereco->endcep != $arrDados['endcep']) {
                $endereco = new Par3_Controller_Endereco();
            }
            $arrDados['endid'] = $endereco->salvarEndereco($arrDados);

            $Secretaria->popularDadosObjeto($arrDados);
            $Secretaria->salvar(true, true);
            $Secretaria->commit();

            simec_redirecionar($url, 'success');

        } catch (Exception $e) {
            $Secretaria->rollback();
            simec_redirecionar($url, 'error');

        }
    }//end salvarInformacoesSecretaria()


    /**
     * Função verificaPreencimentoSecretaria
     * - verifica o preenchimento dos dados da Secretaria e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoSecretaria($inuid)
    {
        $Secretaria = new Par3_Model_InstrumentoUnidadeEntidade();

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO;

        $objSecretaria = $Secretaria->carregarDadosEntidPorTipo($inuid, $tenid);
        $objEndereco = new Par3_Model_Endereco($objSecretaria->endid);

        $perc = 0;
        if ($objSecretaria->entnome != '') $perc++;
        if ($objSecretaria->entemail != '') $perc++;
        if ($objSecretaria->enttelefonecomercial != '') $perc++;

        if ($objEndereco->endcep != '') $perc++;
        if ($objEndereco->endlogradouro != '') $perc++;
        if ($objEndereco->endnumero != '') $perc++;
        if ($objEndereco->endbairro != '') $perc++;

        $perc = $perc / 7 * 100;

        return (int)$perc;

    }//end verificaPreencimentoSecretaria();


    /**
     * Função salvarInformacoesDirigente
     * - salvar informações do dirigente da unidade.
     *
     * @return void.
     *
     */
    public function salvarInformacoesDirigente($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=dirigente";

        $endereco = new Par3_Controller_Endereco();
        $dirigente = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
        $unidade = new Par3_Model_InstrumentoUnidade($arrDados['inuid']);
        $responsabilidade = new Par3_Model_UsuarioResponsabilidade();
        $usuario = new Seguranca_Model_Usuario();

        $entidade = $unidade->inudescricao;
        $estuf = $unidade->estuf;
        $itrid = $unidade->itrid;

        try {
            $arrDados['entcpf'] = str_replace(array('.', '-', '/'), '', $arrDados['entcpf']);
            $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')'), '', $arrDados['enttelefonecomercial']);
            $arrDados['enttelefonecelular'] = str_replace(array('(', '-', ')'), '', $arrDados['enttelefonecelular']);
            $arrDados['enttelefonefax'] = str_replace(array('(', '-', ')'), '', $arrDados['enttelefonefax']);
            $arrDados['entcursomec'] = ($arrDados['entcursomec'] == 't') ? 'TRUE' : 'FALSE';
            $arrDados['entstatus'] = 'A';
            $arrDados['entempoatuacao'] = (int)$arrDados['entempoatuacao'];
            $arrDados['entdtnascimento'] = $arrDados['entdtnascimento'] != '' ? $arrDados['entdtnascimento'] : null;

            $pflcod = ($itrid == 1) ? Par3_Model_UsuarioResponsabilidade::DIRIGENTE_ESTADUAL : Par3_Model_UsuarioResponsabilidade::DIRIGENTE_MUNICIPAL;

            if (!$responsabilidade->validarUsuarioResponsabilidade($arrDados['entcpf'], $pflcod, $unidade->muncod)) {
                $tipo = ($itrid == 1) ? 'Estado' : 'Município';
                $regiao = ($itrid == 1) ? $entidade : $entidade . ' - ' . $estuf;
                throw new \Exception("Este CPF não possui perfil de dirigente para o Município {$tipo} {$regiao}. Para mais informações sobre o procedimento de atualização, acesse a Central de Orientações.");
            }

            if ( ($dirigente->entcpf != $arrDados['entcpf'] && $arrDados['entcpf'] != '') ||
                ($dirigente->entnome != $arrDados['entnome'] && $arrDados['entnome'] != '') ) {
                $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];
                $dirigente->inativaEntidadesInuidPorTipo($arrDados['inuid'], $arrDados['tenid']);
                unset($arrDados['entid']);
                $dirigente = new Par3_Model_InstrumentoUnidadeEntidade();
            }

            $dirigente->popularDadosObjeto($arrDados);
            $dirigente->salvar();
            $dirigente->commit();

//            require_once APPRAIZ.'includes/funcoes_espelhoperfil.php';
//            inserirPerfisSlaves($arrDados['entcpf'], $pflcod);
//            atualizarResponsabilidadesSlaves($arrDados['entcpf'], $pflcod);

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $dirigente->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }//end salvarInformacoesDirigente()


    /**
     * Função salvarInformacoesDirigente
     * - salvar informações do dirigente da unidade.
     *
     * @return void.
     *
     */
    public function salvarInformacoesNutricionistaQuadroTecnico($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=nutricionista";

        try {
            $arrDados['entcpf'] = str_replace(array('.', '-', '/'), '', $arrDados['usucpf2']);
            $arrDados['entnome'] = str_replace(array('(', '-', ')'), '', $arrDados['entnome2']);
            $arrDados['entemail'] = $arrDados['entemail2'];
            $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];

            $nutricionistaQuadroTecnico = new Par3_Model_InstrumentoUnidadeEntidade();

            $nutricionistaQuadroTecnico->popularDadosObjeto($arrDados);

            $nutricionistaQuadroTecnico->salvar();
            $nutricionistaQuadroTecnico->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $nutricionistaQuadroTecnico->rollback();
            simec_redirecionar($url, 'error');

        }
    }

    public function descadastrarInformacoesNutricionista($arrDados) {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=nutricionista";

        try {
            $vinculacao = new Par3_Model_VinculacaoNutricionista();

            $this->inativaEntidade($arrDados);

            $arrVinculacao = $vinculacao->carregarVinculacaoPorInstrumentoUnidade($arrDados['inuid']);

            $vinculacao->excluir($arrVinculacao['vnid']);

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
    public function renomearRT($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=nutricionista";
        try {
            $vinculoNutricionista = new Par3_Model_VinculacaoNutricionista();
            $vinculoNutricionista->nomearRT($arrDados);
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }
    /**
     * Função salvarInformacoesNutricionista
     * - salvar informações do dirigente  e dos membros da equipe da unidade.
     * @return void.
     *
     */
    public function salvarInformacoesNutricionista($arrDados)
    {
        //url de retorno
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu=nutricionista";

        $arrDados['entcpf'] = str_replace(array('.', '-'), '', $arrDados['entcpf']);
        if($arrDados['resetar-senha'] && $arrDados['entcpf'] != '') {
            $modelEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
            $modelEntidade->resetarSenhaNutricionistasPorCPF($arrDados['entcpf']);
            simec_redirecionar($url, 'success');
            return true;
        }
        //atualiza o status de todos os nutricionistas para Quadro técnico 8
        // caso o nutricionista salvo seja cadastrado como responsável
        if($arrDados['tenid'] == 7){
            $modelEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
            $modelEntidade->updateTipoNutricionistaRT($arrDados['inuid']);
        }

        if($arrDados['tenid'] == 8 || $arrDados['tenid'] == 16){
            $arrDados['usucpf'] = $arrDados['entcpf'];
        }

        //tratando dados
        $arrDados['usucpf'] = str_replace(array('.', '-'), '', $arrDados['entcpf']);
        $arrDados['entcpf'] = str_replace(array('.', '-'), '', $arrDados['entcpf']);

        $arrPost['enttelefonecomercial'] = ereg_replace('[()-]','',$arrPost['enttelefonecomercial']);
        $enviar_email = true;
        $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')'),'',$arrDados['enttelefonecomercial']);
        $arrDados['entnome'] = str_replace(array('(', '-', ')'), '', $arrDados['entnome']);
        $arrDados['entemail'] = $arrDados['entemail'];

        $usuario        = new Seguranca_Model_Usuario();
        $controleUnidade= new Par3_Controller_InstrumentoUnidade();
        $nutricionista  = new Par3_Model_DadosNutricionista();

        try {

//            if ($this->validarRepresentanteExistePorCPF($arrDados['entcpf'], $arrDados['inuid'],$arrDados['tenid'])) {
//                throw new \Exception("Este CPF já esta cadastrado como representante desta equipe de nutricionistas.");
//            }
            $arrPost['enttelefonecomercial'] = ereg_replace('[()-]','',$arrDados['enttelefonecomercial']);

//            if ($this->validarRepresentanteExistePorEmail($arrDados['entemail'], $arrDados['inuid'],$arrDados['tenid'])) {
//                throw new \Exception("Este E-mail já esta cadastrado como representante desta equipe de nutricionistas.");
//            }
            //Se o CPF não estão tiver cadastro
            if( !$usuario->recuperarPorCPF($arrDados['entcpf']) && $arrDados['entcpf'] != '' ) {
                $senhageral = 'nut' . substr($arrDados['usucpf'], -3);
                $senha = md5_encrypt_senha($senhageral, '');

                $dadosUsuario['usucpf'] = $arrDados['usucpf'];
                $dadosUsuario['usunome'] = $arrDados['entnome'];
                $dadosUsuario['usuemail'] = $arrDados['entemail'];
                $dadosUsuario['enttelefonecomercial'] = $arrDados['enttelefonecomercial'];
                $dadosUsuario['ususenha'] = $senha;
                $dadosUsuario['usuchaveativacao'] = 'F';
                $dadosUsuario['suscod'] = 'A';

                $usuario->popularDadosObjeto($dadosUsuario);
                $usuario->salvar();
                $usuario->commit();

                $usuarioSistema = new Seguranca_Model_Usuariosistema();
                $usuPar = $usuarioSistema->recuperarTodos('*',array( "usucpf = '{$arrDados['usucpf']}' AND sisid = {$_SESSION['sisid']}"));

                if( empty($usuPar[0]) ) {
                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['enttelefonecomercial'] = $arrDados['enttelefonecomercial'];
                    $arrayUsuarioSistema['sisid'] = $_SESSION['sisid'];
                    $arrayUsuarioSistema['suscod'] = 'B';
                    $arrayUsuarioSistema['pflcod'] = Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();

                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['enttelefonecomercial'] = $arrDados['enttelefonecomercial'];
                    $arrayUsuarioSistema['sisid'] = SISID_PAR;
                    $arrayUsuarioSistema['suscod'] = 'B';
                    $arrayUsuarioSistema['pflcod'] = Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();
                }

                $dadosUsuario['ususenha'] = $senhageral;
                $dadosUsuario['inuid'] = $arrDados['inuid'];
                $itrid = $controleUnidade->pegarItrid($arrDados['inuid']);
                $dadosUsuario['itrid'] = $itrid;

                $dadosPerfil['usucpf'] =  $arrDados['entcpf'];
                $dadosPerfil['pflcod'] =  Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;

                $perfil = new Seguranca_Model_Perfilusuario();
                $perfil->salvarDados($dadosPerfil);

                $dadosPerfil['usucpf'] =  $arrDados['entcpf'];
                $dadosPerfil['pflcod'] =  PAR_PERFIL_NUTRICIONISTA;
                $perfil = new Seguranca_Model_Perfilusuario();
                $perfil->salvarDados($dadosPerfil);

                unset($usuarioPar);

                //enviar email para o primeiro acesso no simec;
                $this->enviarEmailNutricionista($dadosUsuario, 1);
                $enviar_email = false;

            } else {//se já houver CPF cadastrado em usuário sistema
                $usuarioSistema = new Seguranca_Model_Usuariosistema();
                $usuPar = $usuarioSistema->recuperarTodos('*',array( "usucpf = '{$arrDados['usucpf']}' AND sisid = {$_SESSION['sisid']}"));

                if(empty($usuPar[0]) && isset($arrDados['usucpf']) && !empty($arrDados['usucpf']))
                {
                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['sisid'] = $_SESSION['sisid'];
                    $arrayUsuarioSistema['suscod'] = 'B';
                    $arrayUsuarioSistema['pflcod'] = Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();

                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['enttelefonecomercial'] = $arrDados['enttelefonecomercial'];
                    $arrayUsuarioSistema['sisid'] = SISID_PAR;
                    $arrayUsuarioSistema['suscod'] = 'B';
                    $arrayUsuarioSistema['pflcod'] = PAR_PERFIL_NUTRICIONISTA;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();
                }

                unset($usuPar);

                $perfil_usuario = new Seguranca_Model_Perfilusuario();
                $usuPar = $perfil_usuario->recuperarTodos('*',array( "usucpf = '{$arrDados['usucpf']}' AND pflcod = ".Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA));

                if(empty($usuPar[0]))
                {
                    $arrayPerfilUsuario['usucpf'] = $arrDados['usucpf'];
                    $arrayPerfilUsuario['pflcod'] = Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;
                    $arrayPerfilUsuario['sisid'] = $_SESSION['sisid'];
                    $arrayPerfilUsuario['enttelefonecomercial'] = $arrDados['enttelefonecomercial'];
                    $perfil_usuario->salvarDados($arrayPerfilUsuario);

                    $arrayPerfilUsuario['usucpf'] = $arrDados['entcpf'];
                    $arrayPerfilUsuario['pflcod'] = PAR_PERFIL_NUTRICIONISTA;
                    $perfil_usuario->salvarDados($arrayPerfilUsuario);
                }
            }
            $nutricionistaEntidade = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

            if( $nutricionistaEntidade->entcpf == '' ){
                $arrDados['entid']     = $nutricionistaEntidade->recuperarEntidadeUnidadePorCPF($arrDados['inuid'], $arrDados['usucpf'], $arrDados['tenid']);
                $nutricionistaEntidade = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
            }

            $arrayNutricionista['entcpf']    = $arrDados['usucpf'];
            $arrayNutricionista['inuid']     = $arrDados['inuid'];
            $arrayNutricionista['tenid']     = $arrDados['tenid'];
            $arrayNutricionista['entemail']  = $arrDados['entemail'];
            $arrayNutricionista['entnome']   = $arrDados['entnome'];
            $arrayNutricionista['enttelefonecomercial']   = $arrDados['enttelefonecomercial'];
            $arrayNutricionista['entid']     = $arrDados['entid'];
            $arrayNutricionista['entstatus'] = 'A';

            if(($nutricionistaEntidade->entcpf != $arrDados['usucpf']) && !empty($arrDados['entid'])){
                $arrayInativacao['entid'] = $arrDados['entid'];
                $arrayInativacao['menu'] = 'nutricionista';
                $arrayInativacao['inuid'] = $arrDados['inuid'];

                $this->inativaEntidade($arrayInativacao, false);
            }

            $nutricionistaEntidade->popularDadosObjeto($arrayNutricionista);
            $id_entidade = $nutricionistaEntidade->salvar();
            $nutricionistaEntidade->commit();
            $arrayNutricionista['entid'] = $id_entidade;

            $idNutricionista = $nutricionista->recuperarTodos('*',array( "dancpf = '{$arrDados['usucpf']}'"));

            if(empty($idNutricionista[0])){
                $dados_nutrionista['usucpf'] = $arrDados['usucpf'];
                $dados_nutrionista['dnstatus'] = 'A';
                $nutricionista->popularDadosObjeto($dados_nutrionista);
                $nutricionista->salvar();
                $nutricionista->commit();
            }

            //Responsável 7
            //Quadro técnico 8
            //Conselho Infantil 16
            $arrDadosVinvulacao['vncpf']                 = $arrDados['usucpf'];
            $arrDadosVinvulacao['inuid']                 = $arrDados['inuid'];
            $arrDadosVinvulacao['vnstatus']              = "A";
            $arrDadosVinvulacao['snid']                  = 3;
            $arrDadosVinvulacao['tenid']                 = $arrDados['tenid'];
            $arrDadosVinvulacao['enttelefonecomercial']  = $arrDados['enttelefonecomercial'];
            $arrDadosVinvulacao['usucpfalteracao']       = $_SESSION['usucpf'];
            $arrDadosVinvulacao['vndatavinculacao']      = null;
            if($arrDados['tenid'] == 16){$arrDadosVinvulacao['vnatuacaoexclusivaei'] = 't';}

            $vinculacao = new Par3_Model_VinculacaoNutricionista();

            $arrDadosVinvulacao['vnid'] = $vinculacao->carregarVinculacaoNutricionistaPorUnidade($arrDados['usucpf'], $arrDados['inuid']);

            $vinculacao->popularDadosObjeto($arrDadosVinvulacao);

            $vinculacao->salvar(null, null, array('vndatavinculacao'));
            $vinculacao->commit();

            //Busca a siatuação do usuario recem criado
            $dadoUsuario = $usuario->recuperarTodos('suscod', ["usucpf = '{$arrDados['usucpf']}'"]);
            $suscod = ($dadoUsuario[0]['suscod']) ? $dadoUsuario[0]['suscod'] : 'P';


            #BUSCA INFORMAÇÕES PARA SALVAR NO HISTORICO DO USUÁRIO.
            $usuRespon  = new Par3_Model_UsuarioResponsabilidade();
            $unidade    = new Par3_Controller_InstrumentoUnidade();

            $mun_est= $unidade->buscaEstadoMunicipioUnidade($arrDados['inuid']);
            $esfera = $unidade->buscaEsferaUnidade($arrDados['inuid']);

            if( $esfera == 'M' ){
                $dadosRespon['muncod'] = $mun_est;
            }else{
                $dadosRespon['estuf'] = $mun_est;
            }
            $dadosRespon['pflcod'] = Par3_Model_UsuarioResponsabilidade::NUTRICIONISTA;

            $result = $usuRespon->buscarDadosParaHistoricoUsuario( $dadosRespon );

            //cria um historico de atribuição de perfil
            $usuario->cadastrarHistorico([
                'htudsc' => sprintf("Cadastramento como {$result['perfil']} via interface PAR, realizado por {$result['nome']} - CPF: %s, com o perfil {$result['pf_resp']},  associado à: {$result['local']}, na da data de {$result['hoje']}", $_SESSION['usucpf']),
                'usucpf' => $arrDados['usucpf'],
                'sisid'  => 231,
                'suscod' => $suscod
                //'usucpfadm' => $_SESSION['usucpf']
            ]);

            $usuario->cadastrarHistorico([
                'htudsc' => sprintf("Cadastramento como {$result['perfil']} via interface PAR, realizado por {$result['nome']} - CPF: %s, com o perfil {$result['pf_resp']},  associado à: {$result['local']}, na da data de {$result['hoje']}", $_SESSION['usucpf']),
                'usucpf' => $arrDados['usucpf'],
                'sisid'  => SISID_PAR,
                'suscod' => $suscod
                //'usucpfadm' => $_SESSION['usucpf']
            ]);

            if($enviar_email) {
                $this->enviarEmailNutricionista($arrDados, 0);
            }

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $nutricionista->rollback();
            simec_redirecionar($url, 'error',$e->getMessage());

        }
    }//end


    public function salvarInformacoesDirigenteMunicipal($arrDados)
    {

        $usuarioSistema = new Seguranca_Model_Usuariosistema();

        $sql = "SELECT e.entid, ins.muncod, ins.estuf
                        FROM par3.instrumentounidade  ins
			            INNER  JOIN entidade.endereco 			ed ON ed.muncod = ins.muncod
			            INNER JOIN entidade.entidade 			e  ON ed.entid = e.entid
			            INNER JOIN entidade.funcaoentidade 		ef ON ef.entid = e.entid
                        WHERE inuid = {$arrDados['inuid']} AND ef.funid = 1
        				AND  e.entstatus = 'A'";

        $dadosUnidade = $usuarioSistema->carregar($sql)[0];

        //tratando dados
        $arrDados['entcpf'] = str_replace(array('.', '-'), '', $arrDados['entcpf']);
        $arrDados['usucpf'] = $arrDados['entcpf'];

        $enviar_email = true;
        $arrDados['enttelefonecomercial'] = str_replace(array('(', '-', ')'),'',$arrDados['enttelefonecomercial']);
        $arrDados['enttelefonecelular'] = str_replace(array('(', '-', ')'),'',$arrDados['enttelefonecelular']);
        $arrDados['entnome'] = str_replace(array('(', '-', ')'), '', $arrDados['entnome']);
        $arrDados['entemail'] = $arrDados['entemail'];
        $arrDados['entrg'] = str_replace(array('(', '-', ')'), '', $arrDados['entrg']);
        $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];


        //url de retorno
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrDados['inuid']}&menu=dirigente";

        $usuario        = new Seguranca_Model_Usuario();
        $controleUnidade= new Par3_Controller_InstrumentoUnidade();

        try {
            //Se o CPF não estão tiver cadastro
            if( !$usuario->recuperarPorCPF($arrDados['entcpf']) && $arrDados['entcpf'] != '' ){
                $senhageral = 'dir' . substr($arrDados['usucpf'], -3);
                $senha = md5_encrypt_senha($senhageral, '');
                $tel = explode(' ', $arrDados['enttelefonecomercial']);
                $dadosUsuario['usucpf'] = $arrDados['usucpf'];
                $dadosUsuario['usunome'] = $arrDados['entnome'];
                $dadosUsuario['usuemail'] = $arrDados['entemail'];
                $dadosUsuario['usufoneddd'] = $tel[0];
                $dadosUsuario['usufonenum'] = $tel[1];
                $dadosUsuario['ususenha'] = $senha;
                $dadosUsuario['senhageral'] = $senhageral;
                $dadosUsuario['usuchaveativacao'] = 'F';
                $dadosUsuario['suscod'] = 'A';

                $usuario->popularDadosObjeto($dadosUsuario);
                $usuario->salvar();
                $usuario->commit();


                $usuPar = $usuarioSistema->recuperarTodos('*',array( "usucpf = '{$arrDados['usucpf']}' AND sisid = {$_SESSION['sisid']}"));

                if( empty($usuPar[0]) ){
                    $arrayUsuarioSistema['usucpf'] = $arrDados['usucpf'];
                    $arrayUsuarioSistema['sisid'] = SISID_PAR3;
                    $arrayUsuarioSistema['suscod'] = 'A';
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $arrayUsuarioSistema['sisid'] = SISID_OBRAS2;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $arrayUsuarioSistema['sisid'] = SISID_PAR;
                    $usuarioSistema->salvar($arrayUsuarioSistema);
                    $usuarioSistema->commit();
                }

                $dadosResponsabilidade['entid'] =  $dadosUnidade['entid'];
                $dadosResponsabilidade['muncod'] =  $dadosUnidade['muncod'];
                $dadosResponsabilidade['usucpf'] =  $arrDados['usucpf'];

                $dadosResponsabilidade['pflcod'] =  PAR3_PERFIL_DIRIGENTE_MUNICIPAL;
                $sql = "INSERT INTO par3.usuarioresponsabilidade (usucpf, pflcod, entid, muncod)
                        VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}','{$dadosResponsabilidade['muncod']}')";
                $usuarioSistema->executar($sql);
                $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                $usuarioSistema->executar($sql);

                $dadosResponsabilidade['pflcod'] =  OBRAS2_GESTOR_UNIDADE;
                $sql = "INSERT INTO obras2.usuarioresponsabilidade (usucpf, pflcod, entid)
                        VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}')";
                $usuarioSistema->executar($sql);
                $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                $usuarioSistema->executar($sql);

                $dadosResponsabilidade['pflcod'] =  PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO;
                $sql = "INSERT INTO par.usuarioresponsabilidade (usucpf, pflcod, entid, muncod)
                        VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}','{$dadosResponsabilidade['muncod']}')";
                $usuarioSistema->executar($sql);
                $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                $usuarioSistema->executar($sql);

                $dadosUsuario['ususenha'] = $dadosUsuario['senhageral'];
                $this->enviarEmailDirigente($dadosUsuario, 1);

            } else {//se já houver CPF cadastrado em usuário sistema

                $modelUsuariosistema = new Seguranca_Model_Usuariosistema();

                $modelUsuario = new Seguranca_Model_Usuario();

                $modelUsuariosistema->ativaUsuario($arrDados['usucpf'], SISID_PAR3);
                $modelUsuariosistema->ativaUsuario($arrDados['usucpf'], SISID_OBRAS2);
                $modelUsuariosistema->ativaUsuario($arrDados['usucpf'], SISID_PAR);
                $modelUsuariosistema->commit();

                $modelUsuario->ativarUsuario($arrDados['usucpf']);
                $modelUsuario->commit();

                $dadosResponsabilidade['entid'] =  $dadosUnidade['entid'];
                $dadosResponsabilidade['muncod'] =  $dadosUnidade['muncod'];
                $dadosResponsabilidade['usucpf'] =  $arrDados['usucpf'];

                $dadosResponsabilidade['pflcod'] =  PAR3_PERFIL_DIRIGENTE_MUNICIPAL;
                $sql = "SELECT usucpf FROM par3.usuarioresponsabilidade WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}' AND entid = '{$dadosResponsabilidade['entid']}' AND muncod = '{$dadosResponsabilidade['muncod']}' AND rpustatus = 'A'";

                if(!$usuarioSistema->carregar($sql)){
                    $sql = "INSERT INTO par3.usuarioresponsabilidade (usucpf, pflcod, entid, muncod)
                            VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}','{$dadosResponsabilidade['muncod']}')";
                    $usuarioSistema->executar($sql);
                }
                $sql = "SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}'";
                if(!$usuarioSistema->carregar($sql)){
                    $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                    $usuarioSistema->executar($sql);
                }


                $dadosResponsabilidade['pflcod'] =  OBRAS2_GESTOR_UNIDADE;
                $sql = "SELECT usucpf FROM obras2.usuarioresponsabilidade WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}' AND entid = '{$dadosResponsabilidade['entid']}' AND rpustatus = 'A'";
                if(!$usuarioSistema->carregar($sql)) {
                    $sql = "INSERT INTO obras2.usuarioresponsabilidade (usucpf, pflcod, entid)
                        VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}')";
                    $usuarioSistema->executar($sql);
                }
                $sql = "SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}'";
                if(!$usuarioSistema->carregar($sql)){
                    $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                    $usuarioSistema->executar($sql);
                }

                $dadosResponsabilidade['pflcod'] =  PAR_PERFIL_EQUIPE_MUNICIPAL_APROVACAO;
                $sql = "SELECT usucpf FROM par.usuarioresponsabilidade WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}' AND entid = '{$dadosResponsabilidade['entid']}' AND muncod = '{$dadosResponsabilidade['muncod']}' AND rpustatus = 'A'";
                if(!$usuarioSistema->carregar($sql)) {
                    $sql = "INSERT INTO par.usuarioresponsabilidade (usucpf, pflcod, entid, muncod)
                    VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}','{$dadosResponsabilidade['entid']}','{$dadosResponsabilidade['muncod']}')";
                    $usuarioSistema->executar($sql);
                }
                $sql = "SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf = '{$dadosResponsabilidade['usucpf']}' AND pflcod = '{$dadosResponsabilidade['pflcod']}'";
                if(!$usuarioSistema->carregar($sql)){
                    $sql = "INSERT INTO seguranca.perfilusuario (usucpf, pflcod) VALUES ('{$dadosResponsabilidade['usucpf']}','{$dadosResponsabilidade['pflcod']}')";
                    $usuarioSistema->executar($sql);
                }
            }

            $dirigenteEntidade = new Par3_Model_InstrumentoUnidadeEntidade();//$arrDados['entid']);

            $dirigenteEntidade->popularDadosObjeto($arrDados);
            $id_entidade = $dirigenteEntidade->salvar();
            $dirigenteEntidade->commit();

            require_once APPRAIZ.'includes/funcoes_espelhoperfil.php';
            inserirPerfisSlaves($dadosResponsabilidade['usucpf'], $dadosResponsabilidade['pflcod']);
            atualizarResponsabilidadesSlaves($dadosResponsabilidade['usucpf'], $dadosResponsabilidade['pflcod']);

            simec_redirecionar($url, 'success');

            die;
        } catch (Exception $e) {
            $dirigenteEntidade->rollback();
            simec_redirecionar($url, 'error',$e->getMessage());

        }
    }//end

    public function enviarEmailDirigente($dados)
    {
        $controleUnidade         = new Par3_Controller_InstrumentoUnidade();
        $itrDsc = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

        $municipio = $itrDsc;
        $tipoEsfera = 'Município';
        $unidade = $itrDsc;
        $strAssunto =  "Cadastro de Dirigentes  " . $itrDsc ;


        $strMensagem =
            "<pre align=\"center\" style=\"text-align: justify;\"  >
                Você foi cadastrado como Dirigente Municipal de Educação de {$unidade}.
                Por favor, acesse o link a seguir para acessar o SIMEC, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>.

                http://simec.mec.gov.br
                Usuário: {$dados['usucpf']}
                Senha: {$dados['ususenha']}

                Atenciosamente,
                Equipe SIMEC/PAR.
                </pre>";

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
        $strMensagem = html_entity_decode($strMensagem);

        $strEmailTo = $dados['usuemail'];

        enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
    }

    /**
     * Função verificaPreencimentoDirigente
     * - verifica o preenchimento dos dados do dirigente e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreencimentoDirigente($inuid)
    {
        $dirigente = new Par3_Model_InstrumentoUnidadeEntidade();

        $itrid = Par3_Controller_InstrumentoUnidade::pegarItrid($inuid);
        if ($itrid === '2') {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE;
        } else {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO;
        }

        $objDirigente = $dirigente->carregarDadosEntidPorTipo($inuid, $tenid);

        $perc = 0;
        $campos = 6;

        if ($objDirigente->entcpf != '') $perc++;
        if ($objDirigente->entnome != '') $perc++;
        if ($objDirigente->entemail != '') $perc++;
        if ($objDirigente->entrg != '') $perc++;
        if ($objDirigente->entorgexpedidor != '') $perc++;
        if ($objDirigente->enttelefonecomercial != '') $perc++;

        //verificando consistência do cadastro do dirigente municipal (se estiver no contexto municipal)
        if( $itrid == 2 && !$dirigente->verificarDirigenteMunicipal($inuid)){
            //se o cadastro estiver incosistente retorna 0
            $perc = 0;
        }
        else{
            $perc = $perc / $campos * 100;
        }

        return (int)$perc;
    }//end verificaPreencimentoDirigente();


    /**
     * Função verificaPreencimentoEquipe
     * - verifica o preenchimento dos dados da equipe local e retorna um percentual
     * de preenchimento.
     *
     * @return integer $perc.
     *
     */
    public function verificaPreencimentoEquipe($inuid)
    {
        $equipe = new Par3_Model_EquipeLocal();

        $arrPost['inuid'] = $inuid;
        $arrPost['elostatus'] = 'A';
        $arrEquipe = $equipe->carregaArrayEquipe($arrPost);

        $perc = 0;
        $qtd = 0;
        if (is_array($arrEquipe)) {
            foreach ($arrEquipe as $equipe) {
                $qtd++;
                foreach ($equipe as $campo) {
                    if ($campo != '') $perc++;
                }
            }
        }

        if ($perc > 0) $perc = $perc / (6 * $qtd) * 100;

        return (int)$perc;
    }


    /**
     * Verifica o preenchimento de nutricionistas e retorna o percentual numerico
     * @param $inuid
     * @return int
     */
    public function verificaPreencimentoNutricionistas($inuid)
    {
        $entidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $tenid = Par3_Model_InstrumentoUnidadeEntidade::NUTRICIONISTA_RESPONSAVEL;
        $objNutricionista = $entidade->carregarDadosEntidPorTipo($inuid, $tenid);

        $perc = 0;

        if ($objNutricionista->entcpf != '') {
            $perc++;
        }

        if ($objNutricionista->entnome != '') {
            $perc++;
        }

        if ($perc > 0)  {
            $perc = $perc / 2 * 100;
        }

        return (int) $perc;
    }


    /**
     * Função verificaPreencimentoNutriciosistas
     * - verifica o preenchimento dos dados dos nutricionistas e retorna um percentual
     * de preenchimento.
     *
     * @return integer $perc.
     *
     */
    public function verificaPreencimentoConselho($inuid)
    {
        $unidade = new Par3_Model_InstrumentoUnidade($inuid);
        $entidade = new Par3_Model_InstrumentoUnidadeEntidade();

        if($unidade->inusemconselhoeducacao == 't') return 100;

        $itrid = $unidade->itrid;

        if ($unidade->itrid === '2') {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_MUNICIPAL;
        } else {
            $tenid = Par3_Model_InstrumentoUnidadeEntidade::CONSELHO_ESTADUAL;
        }

        $objConselho = $entidade->carregarConselheiros($inuid, $tenid);

        if ($objConselho[0]['entcpf'] != '') $perc++;

        if ($perc > 0) $perc = $perc / 1 * 100;

        return (int)$perc;
    }//end verificaPreencimentoConselho();

    public function carregarConselheiros($inuid, $tenid)
    {
        $entidade = new Par3_Model_InstrumentoUnidadeEntidade();

        return $entidade->carregarConselheiros($inuid, $tenid);

    }//end carregarConselheiros()

    /**
     * Função formNutricionista
     * - monta o formulario de Quadro Técnico/Responsável Técnico Nutricionistas
     *
     * @return string escreve a lista.
     *
     */
    public function formNutricionista($arrayPost,$disabled = '')
    {
        $arrNutricionista = array("entid" => "","entcpf" => "","entnome" => "","entemail" => "","enttelefonecomercial" =>"","tenid" => "");

        if(!empty($arrayPost['entid'])){
            $disabled = "readonly";
            $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade($arrayPost['entid']);
            $arrNutricionista = $instrumentoUnidadeEntidade->recuperarNutricionista($arrayPost);
        }

        $tendsc = $arrNutricionista['tenid']?$arrNutricionista['tenid']:'8';
        global $simec;
        echo '<input type="hidden" name="tenid_old" value="'.$arrNutricionista['tenid'].'"/>';
        echo '<input type="hidden" name="entid" value="'.$arrNutricionista['entid'].'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($arrNutricionista['entcpf']).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $arrNutricionista['entcpf'], array('class' => 'cpf',$disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{".cpf_nutricionista": "no_pessoa_rf"}', 'required'));
        echo $simec->input('entnome', 'Nome',$arrNutricionista['entnome'], array('maxlength' => '255', true, 'readonly' => 'readonly', 'class'=>'cpf_nutricionista'));
        echo $simec->email('entemail', 'E-mail',$arrNutricionista['entemail'], array('class' => 'email', 'required'), Array('required'));
        echo $simec->telefone("enttelefonecomercial","Telefone",$arrNutricionista['enttelefonecomercial'],"");
        echo $simec->radio('tenid','Cargo/Função',$tendsc,array('7' => 'Responsável Técnico','8' => 'Quadro Técnico','16' => 'Exclusivo na Modalidade Educação Infantil'),Array('required'));
        if($arrNutricionista['entcpf']) {
            echo '<div class="row"><div class="pull-right"><button type="submit" class="btn btn-sm btn-warning resetar_senha" name="resetar-senha" value="true" <?php echo $disabled;?><i class="fa fa-refresh"></i> Resetar Senhas</button></div></div>';
        }
    }//end formNutricionista()

    public function formDesvincularNutricionista($arrayPost)
    {
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $arrNutricionista = array("entid" => "","entcpf" => "","entnome" => "","entemail" => "","enttelefonecomercial" =>"","tenid" => "");
        if(!empty($arrayPost['entid'])){
            $arrNutricionista = $instrumentoUnidadeEntidade->recuperarNutricionista($arrayPost);
        }
        global $simec;
        $form['titulo'] = $arrNutricionista['entnome'] .' - '.$arrNutricionista['tendsc'];
        $html  = '<input type="hidden" name="requisicao" value="desvincular-nutricionista">';
        $html .= '<input type="hidden" name="entid" value="'.$arrNutricionista['entid'].'"/>';
        $html .= '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($arrNutricionista['entcpf']).'"/>';
        $html .= $simec->textarea('vnmotivodesvinculacao','Informe o Motivo',"",array("required"));
        $html .= $simec->data('vndatadesvinculacao','Data de desvinculação',"",array("required"));
        $form['html'] = $html;
        return $form;
    }
    /**
     * Função formNovoEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formNovoNutricionista($arrPost)
    {
        global $simec;
        $disabled = '';

        echo '<input type="hidden" name="inuid" value="'.$arrPost['inuid'].'"/>';
        echo $this->formNutricionista($arrPost,$disabled);
    }//end formNovoNutricionista()

    public function recuperarNutricionista($arrayPost)
    {
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $arrNutricionista = $instrumentoUnidadeEntidade->recuperarNutricionista($arrayPost);
        $arrNutricionista['entcpf'] = formatar_cpf($arrNutricionista['entcpf']);
        return $arrNutricionista;
    }
    public function recuperarNutricionistas($arrParam = Array())
    {
        $objEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql = $objEntidade->pegarSQLNutricionistas($arrParam);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('CPF', 'Nome','E-mail','Telefone','Cargo/Função','Situação');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->esconderColunas(['danid']);
        $listagem->setQuery($sql);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('enttelefonecomercial', 'formatar_fone');
        $listagem->addCallbackDeCampo('entcpf', 'formatar_cpf'); // função php para formatar cpf
        $listagem->addCallbackDeCampo('tendsc', 'marcarCampoNegrito');
        if ($disabled == '') {
            $listagem->addAcao('remove', 'inativaIntegrante');
            $listagem->addAcao('edit', 'editarIntegrante');
            $listagem->addAcao('unlinkfa','desvincularIntegrante');
            $listagem->addAcao('eye', array(
                    'func' => 'detalharNutricionista',
                    'extra-params' => array('entcpf','danid'),
                    'titulo' => 'Visualizar dados do Nuricionista'
                ));
        } else {
            $listagem->esconderColunas('id');
        }
        if(!$arrParam['possuiRT'] && $disabled == '') {
            echo "<style>
                    .glyphicon-share{background-color:#1ab394;border: 0px;}
                    .glyphicon-share:hover{background-color:#3ad3b4;border: 0px;}
                  </style>";
            $listagem->addAcao('send2', array('func' => 'nomeiaRT', 'titulo' => 'Promover'));
        }
        //$listagem->turnOffForm();
        $listagem->render();
    }//end listaEquipe()

    public function desvincularNutricionista($arrPost)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=nutricionista";
        try {
            $arrPost['vndatadesvinculacao'] = formata_data_sql($arrPost['vndatadesvinculacao']);
            $vinculo = new Par3_Model_VinculacaoNutricionista();
            $nutricionista = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);
            $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($nutricionista->inuid);
            $vinculo->desvincularNutricionista($arrPost);
            $vinculo->commit();

            //preenchimento de parâmetros para o envio de email
            $dadosEmail = array(
                'entnome'   => $nutricionista->entnome,
                'unidade'   => $instrumentoUnidade->inudescricao,
                'entemail'  => $nutricionista->entemail,
                'mailto'    => $nutricionista->entemail,
                'motivo'    => $arrPost['vnmotivodesvinculacao'],
                'data'      => formata_data($arrPost['vndatadesvinculacao']),
                'assunto'   => "Desvinculação do cadastro de Equipe de Nutricionistas",
            );
            //enviar email informando sobre a desvinculação
            $this->enviarEmailDesvincularNutricionista($dadosEmail);
            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }

    /**
     * Enviar email com login e senha para um novo integrante cadastrado na Equipe Técnica
     * @param $array['entnome','unidade','entemail','senha','mailto', 'assunto', 'mensagem']
     * @return boolean.
     */
    public function enviarEmailDesvincularNutricionista($dadosEmail)
    {
        $strEmailTo  = $dadosEmail['mailto'];
        $strAssunto  = $dadosEmail['assunto'];

        $strMensagem =
            "
                <pre align=\"center\" style=\"text-align: justify;\"  >

               Prezado(a) Sr(a). {$dadosEmail['entnome']},

               Você foi desvinculado no SIMEC, sistema PAR, da equipe de nutricionistas do Município/Estado de {$dadosEmail['unidade']}
               Motivo: ".$dadosEmail['motivo'].".
               Data: ".$dadosEmail['data']."


                Esta é uma mensagem automática e não é necessária nenhuma resposta ou confirmação.

                <b> Ministério da Educação </b>

                </pre>";

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

        $strMensagem = html_entity_decode($strMensagem);

        $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

        return $retorno;
    }

    /**
     * Função recuperarConselheiros
     * - lista conselheiros.
     *
     * @return void.
     *
     */
    public function recuperarConselheiros()
    {
        $objEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql = $objEntidade->pegarSQLNutricionistasQuadroTecnico();

        $cabecalho = array('CPF', 'Nome', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('entcpf', 'formatar_cpf'); // função php para formatar cpf
        $listagem->addAcao('delete', 'inativaIntegranteEquipe');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end recuperarConselheiros()

    /**
     * Função recuperarConselheirosCae
     * - lista conselheiros.
     *
     * @return void.
     *
     */
    public function recuperarEntidadesSimplificado(array $arrPost)
    {
        $objEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql = $objEntidade->montarSQLSimplesEntidades($arrPost);
        $cabecalho = array('CPF','Nome','Função','Tipo','Categoria','E-mail','Arquivo*');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addCallbackDeCampo('arqid', 'setaLinkArquivo');
        $listagem->addCallbackDeCampo('entcpf', 'formatar_cpf'); // função php para formatar cpf
        $listagem->addAcao('edit', 'editarEntidade');
        $listagem->addAcao('delete', 'inativaEntidade');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);

    }//end recuperarConselheirosCae()


    /**
     * Função salvarWorkflowDocumento
     * - Salvar workflow de documento.
     *
     * @return void.
     *
     */
    public function salvarWorkflowDocumento($inuid)
    {
        require_once APPRAIZ.'includes/workflow.php';

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$inuid}";

        $controleUnidade = new Par3_Controller_InstrumentoUnidade();
        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        try {
            if (!$instrumentoUnidade->docid) {
                $docid = wf_cadastrarDocumento(PAR3_WORKFLOW_FLUXO_DO_PAR, "Fluxo do PAR - {$controleUnidade->pegarNomeEntidade($inuid)}");

                $instrumentoUnidade->docid = $docid;
                $instrumentoUnidade->salvar();
                $instrumentoUnidade->commit();
            }
        } catch (Exception $e) {
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }//end salvarWorkflowDocumento()


    /**
     * Função tramitarElaboracao
     * - Tramitar workflow de documento.
     *
     * @return void.
     *
     */
    public function tramitarWorkflowDocumentoNaoFinalizado($inuid, $aedid)
    {
        require_once APPRAIZ.'includes/workflow.php';

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        try {
            wf_alterarEstado($instrumentoUnidade->docid, $aedid, "", array(), array());
            return true;
        } catch (Exception $e) {
            return false;
        }
    }//end tramitarElaboracao()


    /**
     * Função tramitarElaboracao
     * - Tramitar workflow de documento.
     *
     * @return void.
     *
     */
    public function tramitarWorkflowDocumento($inuid, $aedid, $url = 'par3.php?modulo=principal/planoTrabalho/indicadoresQualitativos&acao=A&inuid=%inuid%&finalizado=t' )
    {
        require_once APPRAIZ.'includes/workflow.php';

        $url = str_replace('%inuid%', $inuid, $url);

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        try {
            wf_alterarEstado($instrumentoUnidade->docid, $aedid, "", array(), array());
            simec_redirecionar($url, 'success', 'Diagnóstico tramitado com sucesso');
        } catch (Exception $e) {
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }//end tramitarElaboracao()


//     /**
//      * Função anexarLoudoDiligencia
//      * - Tramitar workflow de documento.
//      *
//      * @return void.
//      *
//      */
//     public function anexarLaudoDiligencia($dados, $files)
//     {
//         if ($files['arqid']){
// 			include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
// 			$file = new FilesSimec("laudo_diligencia", null, "par3");
// 			$file->setUpload('Decreto de Nomeação', 'arqid', false);
// 			$arrDados['arqid'] = $file->getIdArquivo();
// 		}
//     }//end anexarLoudoDiligencia()


    /**
     * Função recuperarEstadoWorkflow
     * - Recuperart estado atual do workflow de documento.
     *
     * @return void.
     *
     */
    public function recuperarEstadoWorkflow($inuid)
    {
        require_once APPRAIZ.'includes/workflow.php';

        $instrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $estado = wf_pegarEstadoAtual($instrumentoUnidade->docid);

        return isset($estado['esdid']) ? $estado['esdid'] : null;
    }//end recuperarEstadoWorkflow


    /**
     * Função salvarInformacoesConselho
     * - salvar informações do conselho.
     *
     * @return void.
     *
     */
    public function salvarInformacoesConselho($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=conselho";

        $conselheiro = new Par3_Model_InstrumentoUnidadeEntidade();
        $endereco = new Par3_Controller_Endereco();
        $unidade = new Par3_Model_InstrumentoUnidade();

        try {

            $arrDados['entcpf'] = str_replace(array('.', '-', '/'), '', $arrDados['entcpf']);
            $arrDados['entusucpfinclusao'] = $_SESSION['usucpf'];

            if ($conselheiro->existeConselheiro($arrDados['entcpf'], $arrDados['inuid'], $arrDados['tenid'])) {
                throw new \Exception("Este CPF já faz parte do conselho.");
            }

            $conselheiro->popularDadosObjeto($arrDados);
            $conselheiro->salvar();
            $conselheiro->commit();

            simec_redirecionar($url, 'success');
        } catch (Exception $e) {
            $conselheiro->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }
    }//end salvarInformacoesConselho


    /**
     * Função inativar conselheiro
     * - inativa membro do conselho.
     *
     * @return void.
     *
     */
    public function inativarConselheiro($arrDados)
    {
        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A" .
            "&inuid={$arrDados['inuid']}&menu=conselho";

        $conselheiro = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);

        try {
            $conselheiro->entstatus = 'I';
            $conselheiro->entdtinativacao = date('Y-m-d H:i:s');
            $conselheiro->entusucpfinativacao = $_SESSION['usucpf'];
            $conselheiro->salvar();
            $conselheiro->commit();

            simec_redirecionar($url, 'success');
            die();
        } catch (Exception $e) {
            $conselheiro->rollback();
            simec_redirecionar($url, 'error');
            die();
        }
    }//end inativarConselheiro()


    /**
     * Função atualiza dados das entidades contidas no SAPE
     * - atualiza entidades que vem do SAPE.
     *
     * @param array $arrDados
     *
     * @return void.
     *
     */

    public function atualizaDadosSAPE($arrDados)
    {
//         $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A".
//                "&inuid={$arrDados['inuid']}&menu={$arrDados['menu']}";

//         $modelEntidade           = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
//         $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($arrDados['inuid']);
//         $modelSAPEDirigente      = new Par3_Model_SAPEDirigente();

//         $arrTenidCNPJ = array(
//             Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA,
//             Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO,
//         );

//         try {
//             switch ($arrDados['tenid']) {
//                 case Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA:
//                     $arrEntidade = $modelSAPEDirigente->retonarEntidade(Par3_Model_SAPEDirigente::SAPE_PREFEITURA, $modelInstrumentoUnidade->estuf, $modelInstrumentoUnidade->muncod);
//                     $arrEntidade['entinscricaoestadual'] = trim($arrEntidade['entinscricaoestadual']) != '' ? $arrEntidade['entinscricaoestadual'] : $modelEntidade->entinscricaoestadual;
//                     $arrEntidade['entsigla']             = trim($arrEntidade['entsigla']) != ''             ? $arrEntidade['entsigla']             : $modelEntidade->entsigla;
//                     break;
//                 case Par3_Model_InstrumentoUnidadeEntidade::PREFEITO:
//                     $arrEntidade = $modelSAPEDirigente->retonarDirigente(Par3_Model_SAPEDirigente::SAPE_PREFEITURA, $modelInstrumentoUnidade->estuf, $modelInstrumentoUnidade->muncod);
//                     $arrEntidade['entdtnascimento']     = trim($arrEntidade['entdtnascimento']) != ''    ? $arrEntidade['entdtnascimento']    : $modelEntidade->entdtnascimento;
//                     $arrEntidade['enttelefonecelular']  = trim($arrEntidade['enttelefonecelular']) != '' ? $arrEntidade['enttelefonecelular'] : $modelEntidade->enttelefonecelular;
//                     $arrEntidade['enttelefonefax']      = trim($arrEntidade['enttelefonefax']) != ''     ? $arrEntidade['enttelefonefax']     : $modelEntidade->enttelefonefax;
//                     break;
//                 case Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO:
//                     if($modelInstrumentoUnidade->itrid == 2){
// //                         $arrEntidade = $modelSAPEDirigente->retonarEntidade(Par3_Model_SAPEDirigente::SAPE_SECRETARIA_MUNICIPAL_EDUCACAO, $modelInstrumentoUnidade->estuf, $modelInstrumentoUnidade->muncod);
//                     }else{
//                         $arrEntidade = $modelSAPEDirigente->retonarEntidade(Par3_Model_SAPEDirigente::SAPE_SECRETARIA_ESTADUAL_EDUCACAO, $modelInstrumentoUnidade->estuf);
//                         $arrEntidade['entsigla'] = trim($arrEntidade['entsigla']) != '' ? $arrEntidade['entsigla'] : $modelEntidade->entsigla;
//                     }
//                     break;
//                 case Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO:
//                     $arrEntidade = $modelSAPEDirigente->retonarDirigente(Par3_Model_SAPEDirigente::SAPE_SECRETARIA_ESTADUAL_EDUCACAO, $modelInstrumentoUnidade->estuf);
//                     $arrEntidade['entdtnascimento']     = trim($arrEntidade['entdtnascimento']) != ''    ? $arrEntidade['entdtnascimento']    : $modelEntidade->entdtnascimento;
//                     $arrEntidade['enttelefonecelular']  = trim($arrEntidade['enttelefonecelular']) != '' ? $arrEntidade['enttelefonecelular'] : $modelEntidade->enttelefonecelular;
//                     $arrEntidade['enttelefonefax']      = trim($arrEntidade['enttelefonefax']) != ''     ? $arrEntidade['enttelefonefax']     : $modelEntidade->enttelefonefax;
//                     break;
//             }

//             if(in_array($arrDados['tenid'], $arrTenidCNPJ)){

//             }

//             if ($arrEntidade) {
//                 $arrEntidade['inuid'] = $arrDados['inuid'];
//                 $arrEntidade['tenid'] = $arrDados['tenid'];
//                 $arrEntidade['booNaoRedireciona'] = true;
//                 $arrEntidade['endbairro'] = $arrEntidade['endbairro'] ? $arrEntidade['endbairro'] : 'não informado';
//                 $arrEntidade['endnumero'] = $arrEntidade['endnumero'] ? $arrEntidade['endnumero'] : '0';
//                 $arrEntidade['enttelefonecomercial'] = $arrEntidade['enttelefonecomercial'] ? substr($arrEntidade['enttelefonecomercial'], 0, 12) : '';
//                 $arrEntidade['enttelefonecelular'] = $arrEntidade['enttelefonecelular'] ? substr($arrEntidade['enttelefonecelular'], 0, 12) : '';
//                 $this->salvarEntidade($arrEntidade);
//             } else {
// //                 simec_redirecionar($url, 'error', 'Não existem dados carregados na tabela para atualizar as infomações.');
//             }

// //             simec_redirecionar($url, 'success', 'Dados atualizados com sucesso!');
//         } catch (Exception $e) {

//             $modelEntidade->rollback();
//             simec_redirecionar($url, 'error', $e);
//         }
    }//end atualizaDadosSAPE()


    /**
     * Função atualiza dados das entidades contidas no SAPE
     * - atualiza entidades que vem do SAPE.
     *
     * @param array $arrDados
     *
     * @return void.
     *
     */
    public function atualizaDadosEXSAPE($arrDados)
    {
//         $modelEntidade           = new Par3_Model_InstrumentoUnidadeEntidade($arrDados['entid']);
//         $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($arrDados['inuid']);
//         $modelSAPEDirigente      = new Par3_Model_SAPEDirigente();

//         $arrTenidCNPJ = array(
//             Par3_Model_InstrumentoUnidadeEntidade::PREFEITURA,
//             Par3_Model_InstrumentoUnidadeEntidade::SECRETARIA_EDUCACAO,
//         );

//         try {
//             switch ($arrDados['tenid']) {
//                 case Par3_Model_InstrumentoUnidadeEntidade::PREFEITO:
//                     $arrEntidade = $modelSAPEDirigente->retonarEXDirigente(Par3_Model_SAPEDirigente::SAPE_PREFEITURA, $modelInstrumentoUnidade->estuf, $modelInstrumentoUnidade->muncod);
//                     break;
//                 case Par3_Model_InstrumentoUnidadeEntidade::DIRIGENTE:
//                     $arrEntidade = $modelSAPEDirigente->retonarEXDirigente(Par3_Model_SAPEDirigente::SAPE_SECRETARIA_MUNICIPAL_EDUCACAO, $modelInstrumentoUnidade->estuf, $modelInstrumentoUnidade->muncod);
//                     break;
//                 case Par3_Model_InstrumentoUnidadeEntidade::SECRETARIO_ESTADUAL_EDUCACAO:
//                     $arrEntidade = $modelSAPEDirigente->retonarEXDirigente(Par3_Model_SAPEDirigente::SAPE_SECRETARIA_ESTADUAL_EDUCACAO, $modelInstrumentoUnidade->estuf);
//                     break;
//             }

//             $arrAtual = $modelEntidade->getDadosEXEntidade( $arrDados['inuid'], $arrDados['tenid'] );

//             if($arrAtual['entcpf'] != ''){
//                 if($arrAtual['entcpf'] == $arrEntidade['entcpf']){
//                     $arrAtual['entnome']                = $arrEntidade['entnome'] == $arrAtual['entnome']                           ? $arrAtual['entnome']              : $arrEntidade['entnome'];
//                     $arrAtual['entemail']               = $arrEntidade['entemail'] == $arrAtual['entemail']                         ? $arrAtual['entemail']             : $arrEntidade['entemail'];
//                     $arrAtual['entrg']                  = $arrEntidade['entrg'] == $arrAtual['entrg']                               ? $arrAtual['entrg']                : $arrEntidade['entrg'];
//                     $arrAtual['entdt_inicio_mandato']   = $arrEntidade['entdt_inicio_mandato'] == $arrAtual['entdt_inicio_mandato'] ? $arrAtual['entdt_inicio_mandato'] : $arrEntidade['entdt_inicio_mandato'];
//                     $arrAtual['entdt_fim_mandato']   	= $arrEntidade['entdt_fim_mandato'] == $arrAtual['entdt_fim_mandato']       ? $arrAtual['entdt_fim_mandato']    : $arrEntidade['entdt_fim_mandato'];
//                     $arrAtual['entusucpfinclusao'] = '00000000191';
//                     $arrAtual['entstatus'] = 'I';
//                 }
//             }else{
//                 $arrAtual = $arrEntidade;
//                 $arrAtual['entsexo'] = strtoupper($arrEntidade['entsexo']);
//                 $arrAtual['tenid'] = $arrDados['tenid'];
//                 $arrAtual['inuid'] = $arrDados['inuid'];
//                 $arrAtual['entusucpfinclusao'] = '00000000191';
//                 $arrAtual['entstatus'] = 'I';
//                 $arrAtual['entdtinativacao'] = $arrAtual['entdt_fim_mandato'];
//             }

//             $modelEntidade = new Par3_Model_InstrumentoUnidadeEntidade($arrAtual['entid']);
//             $modelEntidade->popularDadosObjeto($arrAtual);
//             $modelEntidade->salvar();
//             $modelEntidade->commit();

//         } catch (Exception $e) {

//             $modelEntidade->rollback();
//             simec_redirecionar($url, 'error', $e);
//         }
    }//end atualizaDadosSAPE()


    /**
     * Função formHistorico
     * - inativa membro do conselho.
     *
     * @return void.
     *
     */
    public function formHistorico($arrDados)
    {
        $model = new Par3_Model_InstrumentoUnidadeEntidade();
        $arrHistorico = $model->carregaArrayHistoricoEntidade($arrDados);

        if (count($arrHistorico) > 0 && is_array($arrHistorico)) {
            if ($arrDados['booCPF']) {
                $historico['id'] = formatar_cpf($historico['id']);
                $labelId = 'CPF';
            }
            if (!$arrDados['booCPF']) {
                $historico['id'] = formatar_cnpj($historico['id']);
                $labelId = 'CNPJ';
            }

            require_once APPRAIZ . '/par3/modulos/principal/listaHistorico.inc';
        }
    }//end formHistorico()

    public function enviarEmailNutricionista($dados, $novo)
    {
        $controleUnidade         = new Par3_Controller_InstrumentoUnidade();
        $itrDsc = $controleUnidade->pegarNomeEntidade($_REQUEST['inuid']);

        if($dados['itrid' == '2']) {
            $municipio = $itrDsc;
            $tipoEsfera = 'Município';
            $unidade = $itrDsc;
            $strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar  " . $itrDsc ;
        } else {
            $tipoEsfera = 'Estado';
            $unidade = $itrDsc;
            $strAssunto =  "Cadastro de Nutricionista para Programa Alimentação Escolar  " . $itrDsc ;
        }

        if ($novo) {
            $strMensagem =
                "<pre align=\"center\" style=\"text-align: justify;\"  >
Você foi cadastrado como Nutricionista no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".
 
http://simec.mec.gov.br
Usuário: {$dados['usucpf']}
Senha: {$dados['ususenha']}
 
Em caso de dúvidas, entrar em contato pelo PAR Fale Conosco, no endereço www.fnde.gov.br/parfaleconosco/index.php/publico ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
 
 
Atenciosamente,
Coordenação de Segurança Alimentar e Nutricional - SIMEC/PAR
</pre>";
        } else {
            $strMensagem = "<pre align=\"center\" style=\"text-align: justify;\"  >
Você foi cadastrado como Nutricionista no âmbito do Programa Nacional de Alimentação Escolar no {$tipoEsfera} de {$unidade}.
Por favor, acesse o link a seguir para acessar o SIMEC/Módulo PAR, insira seu CPF, senha atuais de cadastro no SIMEC e clique no botão <span style=\"width: 100px; color: white; background-color:#47A447; border: 0px;\"  >  Entrar  </span>. Confirme que
você atua nesta função clicando no botão \"Validar\".  Caso não atue nesta função, escolha a opção \"Não validar\".
 
http://simec.mec.gov.br
 
 
Caso não lembre a sua senha acesse o link http://simec.mec.gov.br/recupera_senha.php
Se mesmo assim não conseguir acessar o sistema, entrar em contato pelo PAR Fale Conosco, no endereço www.fnde.gov.br/parfaleconosco/index.php/publico ou no sítio do FNDE em Programas>PAR>Contatos>Acesso para usuário público.
Equipe Técnica do PAR - Plano de Ações Articuladas
 
 
 
Atenciosamente,
Coordenação de Segurança Alimentar e Nutricional - SIMEC/PAR
</pre>";
        }

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
        $strMensagem = html_entity_decode($strMensagem);

        if(!empty($dados['entemail'])) {
            $strEmailTo = $dados['entemail'];
        }else{
            $strEmailTo = $dados['usuemail'];
        }

        if($_SERVER['HTTP_HOST'] == "dsv-simec" || $_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br")
        {

            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

        }
        else if($_SERVER['HTTP_HOST'] == "simec-local" )
        {
            //ver($strMensagem,d);
        }
        else
        {

            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }
    }

    public function validacaoNutricionista($dados){

        $vnid		= $dados['vnid'];
        $snid		= $dados['snid'];

        $vinculacaoNutri = new Par3_Model_VinculacaoNutricionista($vnid);

        // Verifica, caso esteja não validando ele irá inativar também
        if($snid == 2)
        {
            $dadosVinculacao['vnstatus']            = 'I';
            $dadosVinculacao['usucpfalteracao']     = $_SESSION['usucpf'];
            $dadosVinculacao['vndatadesvinculacao'] ="'now()'";

            $nutricionista = new Par3_Model_InstrumentoUnidadeEntidade();

            $entid = $nutricionista->recuperarEntidadeUnidadePorCPF($vinculacaoNutri->inuid, $vinculacaoNutri->vncpf, $vinculacaoNutri->tenid);

            $nutricionista = new Par3_Model_InstrumentoUnidadeEntidade($entid);

            $nutricionista->entstatus = 'I';
            $nutricionista->salvar();

        }
        else if($snid == 6)
        {
            $dadosNutri = new Par3_Model_DadosNutricionista();
            $dadosNutri->carregarDadosPorCPF($_SESSION['usucpf']);
            $vinculacaoStatus = '';
        }
        else
        {
            $vinculacaoStatus = '';
        }

        // Executa a mudança
        if(!empty($dadosVinculacao['vnstatus'])){
            $vinculacaoNutri->vnstatus =  $dadosVinculacao['vnstatus'];
        }

        if(!empty($dadosVinculacao['usucpfalteracao'])){
            $vinculacaoNutri->usucpfalteracao =  $dadosVinculacao['usucpfalteracao'];
        }

        if(!empty($dadosVinculacao['vndatadesvinculacao'])){
            $vinculacaoNutri->vndatadesvinculacao =  $dadosVinculacao['vndatadesvinculacao'];
        }

        $vinculacaoNutri->snid =  $snid;

        $vinculacaoNutri->salvar();

        // Caso ocorra tudo certo com a solicitação
        if($vinculacaoNutri->commit())
        {
            // Caso esteja inativando
            if($snid == 2)
            {
                $dadosNutri = $vinculacaoNutri->getNutriPorVinculacao($vnid);

                $inuid 		= $dadosNutri['inuid'];
                $nomeNutri	= $dadosNutri['entnome'];
                $tenid = $dadosNutri['tenid'];

                if($tenid == 7)
                {
                    $cargo = "Responsável Técnico/Nutricionista";
                }
                else
                {
                    $cargo = "Quadro Técnico/Nutricionistas";
                }

                $InstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
                $InstrumentoUnidadeEntidade->inativaEntidadesInuidPorTipo($inuid,$tenid);

                if($InstrumentoUnidadeEntidade->commit())
                {
                    if($inuid)
                    {
                        $arrEmail 	= $vinculacaoNutri->getEmailRespNutriVinc($inuid);

                        if($arrEmail)
                        {
                            $email		= $arrEmail['email'];
                            $esfera		= $arrEmail['esfera'];

                            $strAssunto = "Aviso de desvinculação de nutricionista";

                            $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

                            $strMensagem ="<pre align=\"center\" style=\"text-align: justify;\"  >
Informamos que o(a) nutricionista {$nomeNutri} não validou o vínculo indicado pela entidade como  {$cargo}
no cadastro do Programa Nacional de Alimentação Escolar nesta data. Observe se o número de nutricionistas vinculados ao seu {$esfera}
está de acordo com os parâmetros numéricos definidos na Resolução CFN nº 465/2010.

http://simec.mec.gov.br

Atenciosamente,
Equipe SIMEC/PAR.

                                </pre>";
                            if( $_SERVER['HTTP_HOST'] == "simec-local" || $_SERVER['HTTP_HOST'] == "localhost" )
                            {
                                echo 'sucesso';
                                die();
                            }
                            elseif($_SERVER['HTTP_HOST'] == "simec-d" || $_SERVER['HTTP_HOST'] == "simec-d.mec.gov.br")
                            {
                                $strEmailTo = array('andreneto@mec.gov.br', 'manuelita.brito@mec.gov.br', 'deborah.silva@fnde.gov.br' , 'eliene.sousa@fnde.gov.br', 'maria.neres@fnde.gov.br', 'olavo.braga@fnde.gov.br', 'irisleia.silva@fnde.gov.br','elias.oliveira@mec.gov.br', 'Murilo.Martins@mec.gov.br');
                                $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
                                echo 'sucesso';
                                die();
                            }
                            else
                            {
                                $strEmailTo = $email;
                                $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
                                echo 'sucesso';
                                die();
                            }
                        }
                        else
                        {
                            echo 'Erro ao enviar Email';
                            die();
                        }
                    }
                    else
                    {
                        echo 'Erro ao enviar Email';
                        die();
                    }
                }
                else
                {
                    echo 'Erro ao excluir vinculação do Nutricionista';
                    die();
                }
            }
            else
            {
                echo 'sucesso';
                die();
            }
        }
        else
        {
            echo 'Erro ao setar validação';
            die();
        }
        die();
    }

    public function calcularUnidadeEntidades($arrPost)
    {
        $mUnidadeEntidade =  new Par3_Model_InstrumentoUnidadeEntidade();
        return $mUnidadeEntidade->calcularUnidadeEntidades($arrPost);
    }

    /**
     * Validar se o integrante já faz parte da equipe tecnica
     * - Verificar integrante existente.
     *
     * @return boolean.
     *
     */
    public function validarRepresentanteExistePorCPF($usucpf, $inuid,$tenid) {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $resultado = $modelInstrumentoUnidade->recuperarEntidadeUnidadePorCPF($inuid, $usucpf, $tenid);

        return $resultado != '' ? true : false;
    }

    /**
     * Validar se o integrante já faz parte da equipe tecnica
     * - Verificar integrante existente.
     *
     * @return boolean.
     *
     */
    public function validarRepresentanteExistePorEmail($entemail, $inuid,$entid) {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $resultado = $modelInstrumentoUnidade->recuperarEntidadeUnidadePorEmail($entemail, $inuid,$entid);

        return $resultado!= '' ? true : false;
    }

    public function listaDirigenteMunicipioAtivos($arrPost)
    {
        $modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $stColunas = "
            t1.entid,
            t1.entcpf,
            t1.entnome,
            t1.entemail,
            t1.enttelefonecomercial,
            t1.entdt_inicio_mandato,
            CASE WHEN exists (SELECT suscod FROM seguranca.usuario WHERE usucpf = t1.entcpf AND suscod = 'B') THEN 'Bloqueado' else entstatus END as entstatus,
	        CASE WHEN exists (SELECT suscod FROM seguranca.usuario WHERE usucpf = t1.entcpf AND suscod = 'B') THEN '<button type=\"button\" class=\"btn btn-warning\" style=\"width:110px;\" id=\"pdf\" onclick=\"desbloquearDirigente('''||t1.entcpf||''');\"></i> Desbloquear </button>' END as botaoDesbloqueio
        ";

        $i = 0;

        if ($arrPost['entnome']) {
            $arFiltro[] = "entnome ilike ('%" . sanitizar_string_pesquisa($arrPost['entnome']) . "%')";
        }
        if ($arrPost['entcpf']) {
            $arrPost['entcpf'] = limpar_numero($arrPost['entcpf']);
            $arFiltro[] = "entcpf = '{$arrPost['entcpf']}' ";
        }

        if($arrPost['entstatus']){
            $arFiltro[] = "entstatus = '{$arrPost['entstatus']}' ";
        }

        $arFiltro[] = "entstatus = 'A'";
        $arFiltro[] = "entcpf IS NOT NULL";
        $arFiltro[] = "inuid = '{$arrPost['inuid']}'";
        $arFiltro[] = "tenid = '{$arrPost['tenid']}'";
        $RsQuery = $modelInstrumentoUnidadeEntidade->recuperarTodos($stColunas, $arFiltro, '', array('query' => true));
        $arCabecalho = array(
            'CPF',
            'Nome',
            'E-mail',
            'Telefone',
            'Data de Nomeação',
            'Situação PAR',
            'Desbloqueio'
        );

        $arCampos = array(
            'entcpf',
            'entnome',
            'entemail',
            'enttelefonecomercial',
            'entdt_inicio_mandato',
            'entstatus'
        );

        $esconderColunas = array('entid');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($arCabecalho);
        $listagem->setTitulo('Dirigente Ativo');
        $listagem->setId('dirigentes_ativos');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($RsQuery);
        //$listagem->setOrdenacao(false);
        //$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('enttelefonecomercial', 'formatar_fone');
        $listagem->addCallbackDeCampo('entcpf', 'formatar_cpf');
        $listagem->addCallbackDeCampo('entdt_inicio_mandato', 'formata_data');
        $listagem->addCallbackDeCampo('entstatus', 'formata_status');

        $listagem->addAcao('view', 'visuaizarDirigente');
        $listagem->addAcao('edit', 'editarDirigente');
        $listagem->addAcao('remove', 'removerDirigente');

        $listagem->turnOnOrdenacao();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

    }

    public function listaDirigenteMunicipio($arrPost, $boolImprimir = false)
    {
        $modelInstrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();

        $stColunas = "
            t1.entid,
            t1.entcpf,
            t1.entnome,
            t1.entemail,
            t1.enttelefonecomercial,
            t1.entdt_inicio_mandato,
            t1.entdt_fim_mandato,
            t1.entstatus
        ";

        $i = 0;

        if ($arrPost['entnome']) {
            $arFiltro[] = "entnome ilike ('%" . sanitizar_string_pesquisa($arrPost['entnome']) . "%')";
        }
        if ($arrPost['entcpf']) {
            $arrPost['entcpf'] = limpar_numero($arrPost['entcpf']);
            $arFiltro[] = "entcpf = '{$arrPost['entcpf']}' ";
        }
        if (!$boolImprimir) {
            $arFiltro[] = "entstatus = 'I' ";
        }

        $arFiltro[] = "entcpf IS NOT NULL";
        $arFiltro[] = "inuid = '{$arrPost['inuid']}' ";
        $arFiltro[] = "tenid = '{$arrPost['tenid']}' ";
        $RsQuery = $modelInstrumentoUnidadeEntidade->recuperarTodos($stColunas, $arFiltro, 'entstatus', array('query' => true));

        $arCabecalho = array(
            'CPF',
            'Nome',
            'E-mail',
            'Telefone',
            'Data de Nomeação',
            'Data Fim do Mandato',
            'Situação PAR'
        );

        $arCampos = array(
            'entcpf',
            'entnome',
            'entemail',
            'enttelefonecomercial',
            'entdt_inicio_mandato',
            'entdt_fim_mandato',
            'entstatus'
        );

        $esconderColunas = array('entid');

        switch ($arrPost['req']) {
            case 'xls':
                $tipoRelatorio = Simec_Listagem::RELATORIO_XLS;
                $tratamentoListaVazia = Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
                break;
            case 'pdf':
                $tipoRelatorio = Simec_Listagem::RELATORIO_CORRIDO;
                $tratamentoListaVazia = Simec_Listagem::TOTAL_SEM_TOTALIZADOR;
                break;
            default:
                $tipoRelatorio = Simec_Listagem::RELATORIO_PAGINADO;
                $tratamentoListaVazia = Simec_Listagem::SEM_REGISTROS_MENSAGEM;
                break;
        }

        $listagem = new Simec_Listagem($tipoRelatorio);
        $listagem->setCabecalho($arCabecalho);
        //$listagem->setOrdenacao(false);
        $listagem->setTitulo('Dirigentes');
        $listagem->setId('dirigentes_inativo');
        $listagem->esconderColunas($esconderColunas);
        $listagem->setQuery($RsQuery);
        $listagem->setTamanhoPagina(10);
        $listagem->addCallbackDeCampo('enttelefonecomercial', 'formatar_fone');
        $listagem->addCallbackDeCampo('entcpf', 'formatar_cpf');
        $listagem->addCallbackDeCampo('entdt_inicio_mandato', 'formata_data');
        $listagem->addCallbackDeCampo('entstatus', 'formata_status');
        $listagem->addCallbackDeCampo('entdt_fim_mandato', 'formata_data');

        if($arrPost['req'] != 'pdf' && $arrPost['req'] != 'xls'){
            $listagem->addAcao('view', 'visuaizarDirigente');
        }

        $listagem->turnOnOrdenacao();
        $listagem->render($tratamentoListaVazia);

    }
// funcção para buscar Município
    public function buscaMunicipio($cnpj,$identificador,$inuid = null)
    {
        //Implementação realizada para trazer somente o endereço do instrumento - unidade selecionado
        $whereInuid = $inuid?"AND iu.inuid = {$inuid}":'';
        //
        global $db;

        if ($identificador == 'prefeitura' || $identificador == 'secretaria') {
            $sql = "
        	SELECT DISTINCT mun.mundsc
                FROM par3.instrumentounidade_entidade ie
                INNER JOIN par3.instrumentounidade iu ON iu.inuid = ie.inuid
                inner join municipio mun on mun.muncod = iu.muncod
                where ie.entcnpj = '$cnpj' {$whereInuid}";

        }else if($identificador == 'prefeito'){
            $sql = "SELECT DISTINCT mun.mundsc
                FROM par3.instrumentounidade_entidade ie
                INNER JOIN par3.instrumentounidade iu ON iu.inuid = ie.inuid
                inner join municipio mun on mun.muncod = iu.muncod
                where ie.entcpf = '$cnpj'  {$whereInuid}";

        }
        $dados = $db->carregar($sql);

        return $dados[0];


    }

    /**
     * Listar nutricionistas pendentes
     * alterar status de acesso
     * resetar a senha
     */
    public function resetarSenhas()
    {
        $mEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $rsNutricionistas = $mEntidade->recuperarNutricionistasPendentes();
        foreach($rsNutricionistas as $nutricionista) {
            $mEntidade->resetarSenhaNutricionistasPorCPF($nutricionista['entcpf']);
        }

        $stTableNutricionistas  = '<table class="table table-responsive table-stripped" style="border: 1px solid black;border-collapse: collapse;text-align: justify;" border="3">';
        $stTableNutricionistas .= '<tr>';
        $stTableNutricionistas .= '<td align="center">QTD</td>';
        $stTableNutricionistas .= '<td align="center">Nome</td>';
        $stTableNutricionistas .= '<td align="center">CPF</td>';
        $stTableNutricionistas .= '<td align="center">Tipo</td>';
        $stTableNutricionistas .= '<td align="center">Situação</td>';
        $stTableNutricionistas .= '</tr>';

        $cont = 1;

        foreach($rsNutricionistas as $nutricionista) {
            $entcpf = formatar_cpf($nutricionista['entcpf']);
            $stTableNutricionistas .= "<tr>";
            $stTableNutricionistas .= "<td align='center'>{$cont}</td>";
            $stTableNutricionistas .= "<td align='center'>{$nutricionista['entnome']}</td>";
            $stTableNutricionistas .= "<td align='center'>{$entcpf}</td>";
            $stTableNutricionistas .= "<td align='center'>{$nutricionista['tendsc']}</td>";
            $stTableNutricionistas .= "<td align='center'>{$nutricionista['sndescricao']}</td>";
            $stTableNutricionistas .= "</tr>";
            $cont++;
        }

        $stTableNutricionistas .= '</table>';

        $strMensagem = <<<EMAIL
    <pre align=\"center\" style=\"text-align: justify;\">
        <p>Lista de Nutricionistas com senhas Atualizadas:</p>
        {$stTableNutricionistas}
    </pre>
EMAIL;

        if ($_SERVER['HTTP_HOST'] == "simec-dsv") {
            return $stTableNutricionistas;
//            die;
        }
        $strEmailTo  = array_filter(array('daniel.fiuza@mec.gov.br',$_SESSION['usuemail']));
        if ($_SERVER['HTTP_HOST'] == "dsv-simec.mec.gov.br") {
            $strAssunto  = 'Nutricionistas com senhas reenviadas';
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }else {
            $strAssunto  = 'Nutricionistas com senhas reenviadas';
            $remetente   = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");
            $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);
        }

        return $stTableNutricionistas;
    }
}