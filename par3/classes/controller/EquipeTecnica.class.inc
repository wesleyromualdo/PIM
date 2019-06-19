<?php

/**
 * Classe de controle da equipe local
 *
 * @category Class/Render
 * @package  A1
 * @author   Daniel Fiuza <danielfiuza@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 14/02/2017
 * @link     no link
 */

class Par3_Controller_EquipeTecnica extends Controle
{
    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formEquipeTecnica($arrPost = null,$disabled = '')
    {
        global $simec;

        $sqlSeguimento = Par3_Model_EquipeLocalSeguimento::pegarSQLSelectCombo();
        $sqlCargo = Par3_Model_EquipeLocalCargo::pegarSQLSelectCombo();

        $arrIntegranteEquipe = array("entid" => "","entcpf" => "","entnome" => "","entemail" => "","enttelefonecomercial" =>"");
        if($arrPost['entid']){
            $disabled = "readonly";
           $arrIntegranteEquipe =  $this->recuperarIntegranteEquipeTecnica($arrPost);
        }

        echo '<input type="hidden" name="entid" value="'.$arrIntegranteEquipe['entid'].'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($arrIntegranteEquipe['entcpf']).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $arrIntegranteEquipe['entcpf'], array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"#entnome": "no_pessoa_rf"}', 'required'));
        echo $simec->input('entnome', 'Nome', $arrIntegranteEquipe['entnome'], array('maxlength' => '255','readonly', 'required'));
        echo $simec->telefone('enttelefonecomercial','Telefone', $arrIntegranteEquipe['enttelefonecomercial'],array('class' => 'telefone'));
        echo $simec->email('entemail', 'E-mail', $arrIntegranteEquipe['entemail'], array('class' => 'email',"", 'required'));
    }//end formEquipeLocal()


    /**
     * Função formNovoEquipeLocal
     * - monta o formulario de equipe local
     *
     * @return string escreve a lista.
     *
     */
    public function formNovoEquipeTecnica($arrPost)
    {
        global $simec;
        $disabled = null;

        echo '<input type="hidden" name="inuid" value="'.$arrPost['inuid'].'"/>';
        echo '<input type="hidden" name="tenid" value="'.Par3_Model_InstrumentoUnidadeEntidade::EQUIPE_TECNICA.'"/>';
        echo $this->formEquipeTecnica($arrPost,$disabled);
    }//end formNovoEquipeTecnico()
    /**
     * Função listaEquipe
     * - monta a lista de equipe técnica.
     *
     * @return escreve a lista.
     *
     */
    public function listaEquipe($arrPost)
    {
        global $disabled;
        $modelUnidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql = $modelUnidade->montarSQLSimplesEntidades($arrPost);

        $disabled = 'disabled';
        if (Par3_Model_UsuarioResponsabilidade::perfilGestorUnidade()) $disabled = '';

        $cabecalho = array('CPF', 'Nome','Telefone', 'E-mail');

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->esconderColunas(['id','cacfuncao','cactipoocupacao','cctdsc','arqid']);
        $listagem->addCallbackDeCampo('entcpf','formatar_cpf'); // função php para formatar cpf
        $listagem->addCallbackDeCampo('enttelefonecomercial','formatar_fone'); // função php para formatar telefone
        if($disabled == ''){
            $listagem->addAcao('remove', 'inativaIntegranteEquipe');
            $listagem->addAcao('edit', 'editaIntegrante');
        }
        $listagem->turnOnOrdenacao();
        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::TOTAL_SEM_TOTALIZADOR);
    }//end listaEquipe()


    /**
     * Função salvar
     * - salvar novo membro da equipe.
     *
     * @return void.
     *
     */
    public function salvar($arrPost)
    {
        $modelInstrumentoUnidadeEntidade  = new Par3_Model_InstrumentoUnidadeEntidade();
        $modelSegurancaUsuario            = new Seguranca_Model_Usuario();
        $modelSegurancaUsuarioSistema     = new Seguranca_Model_Usuario_sistema();
        $modelSegurancaPerfilUsuario      = new Seguranca_Model_Perfilusuario();
        $modelSegurancaHistoricoUsuario   = new Seguranca_Model_Historicousuario();
        $modelInstrumentoUnidade          = new Par3_Model_InstrumentoUnidade();
        $modelConfiguracoesPar            = new Par3_Model_ConfiguracoesPar();
        $modelUsuarioRepsonsabilidade     = new Par3_Model_UsuarioResponsabilidade();

        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=tecnico";
        $retornoEmail = "";
        try {
            if($this->limiteIntegrantesExcedido($arrPost)&&empty($arrPost['entid'])){//verifica se foi excedido o limite de integrantes
                throw new \Exception("Máximo de integrantes atingido");
            }

            $arrPost['entcpf'] = str_replace(array('.', '-'), '', $arrPost['entcpf']);
            $cpf4Ultimos = substr($arrPost['entcpf'],7,5);

            if (empty($arrPost['entid'])) {
                if($this->validarRepresentanteExistePorCPF($arrPost['entcpf'], $arrPost['inuid'],$arrPost['tenid'])){
                  throw new \Exception("O usuário com este CPF já esta cadastrado como integrante desta equipe técnica.");
                }
            }

            $arrPost['enttelefonecomercial'] = ereg_replace('[()-]','',$arrPost['enttelefonecomercial']);

            if (empty($arrPost['entid'])) {
                if ($this->validarRepresentanteExistePorEmail($arrPost['entemail'], $arrPost['inuid'], $arrPost['tenid'])) {
                    throw new \Exception("Este E-mail já esta cadastrado como integrante desta equipe técnica.");
                }
            }

            if(!$arrPost['entid']){
                $arrPost['entusucpfinclusao'] = $_SESSION['usucpf'];
            }

            $dadosUnidade = $modelInstrumentoUnidade->getDadosUnidade($arrPost['inuid']);
            $arrPost['unidade'] = $dadosUnidade['inudescricao'];

            $modelInstrumentoUnidadeEntidade->popularDadosObjeto($arrPost);
            $arrPost['entid'] = $modelInstrumentoUnidadeEntidade->salvar();

            //caso não haja o cpf cadastrado em usuario
            $usuario = $modelSegurancaUsuario->recuperarPorCPF($arrPost['entcpf']);
            if(!$usuario){ //Salvando dados em seguranca.usuario caso usuario não tenha cadastro

                //recuperar dados da Entidade
                $dadosUnidade['tpocod']  = $dadosUnidade['itrid'] == 2 ? 3 : 2;
                $dadosUnidadade['carid'] = $dadosUnidade['itrid'] == 2 ? 5 : 4;
                $mEntidade  = new Entidade_Model_Entidade();
                $arrEntidade  = $mEntidade->retornaEntidade($dadosUnidade);
                //ver($arrEntidade,d);

                $arrSegUsuario = array(
                    'usunome'           => $arrPost['entnome'],
                    'usucpf'            => $arrPost['entcpf'],
                    'usuemail'          => $arrPost['entemail'],
                    'suscod'            => 'A',
                    'ususenha'          => md5_encrypt_senha($cpf4Ultimos, ''),
                    'usuchaveativacao'  => 'FALSE',
                    'regcod'            => $dadosUnidade['estuf'],
                    'muncod'            => $dadosUnidade['muncod'],
                    'tpocod'            => $dadosUnidade['tpocod'],
                    'entid'             => $arrEntidade['codigo'],
                    'carid'             => $dadosUnidadade['carid']
                );

                $modelSegurancaUsuario->popularDadosObjeto($arrSegUsuario);
                $modelSegurancaUsuario->salvar();

                //mandar email $array['entnome','unidade','entemail','senha','remetente', 'mailto', 'assunto']
                $dadosEmail = array(
                    'entnome'   => $arrPost['entnome'],
                    'unidade'   => $arrPost['unidade'],
                    'entemail'  => $arrPost['entemail'],
                    'senha'     => $cpf4Ultimos,
                    'mailto'    => $arrPost['entemail'],
                    'assunto'   => "Cadastro de Integrante em Equipe Técnica",
                );
                $retornoEmail = $this->enviarEmailNovoIntegrante($dadosEmail);
            }

            //caso não haja o cpf cadastrado em usuariosistema
            $sql = "SELECT * FROM seguranca.usuario_sistema WHERE usucpf = '{$arrPost['entcpf']}' AND sisid = 231";
            $usuarioSistema = $modelSegurancaUsuarioSistema->recuperar($sql);
            if(!$usuarioSistema){ //Salvando dados em seguranca.usuario_sistema caso usuario não tenha cadastro
                $arrSegUsuario = array(
                    'usucpf' => $arrPost['entcpf'],
                    'sisid'  => 231,'susstatus' => 'A',
                    'suscod' => 'A'
                );
//                $modelSegurancaUsuarioSistema->popularDadosObjeto($arrSegUsuario);
                $modelSegurancaUsuarioSistema->executar("INSERT INTO seguranca.usuario_sistema(usucpf,sisid,suscod) values('{$arrSegUsuario['usucpf']}',231,'A')");
//                $modelSegurancaUsuarioSistema->salvar();
            }

            //verificar se há cpf cadastrado com o perfil estadual ou municipal
            $pflcod = /*$arrPost['tcpid'] == 1*/ $dadosUnidade['itrid'] == 2 ? Par3_Model_UsuarioResponsabilidade::EQUIPE_MUNICIPAL : Par3_Model_UsuarioResponsabilidade::EQUIPE_ESTADUAL;
            $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '{$arrPost['entcpf']}' AND pflcod = {$pflcod}";
            $modelSegurancaPerfilUsuario->executar($sql);
            $sql = "INSERT INTO seguranca.perfilusuario(usucpf,pflcod) values('{$arrPost['entcpf']}',{$pflcod})";
            $modelSegurancaPerfilUsuario->executar($sql);
            $modelSegurancaPerfilUsuario->commit();

            //gravar no histórico
            $sql = "SELECT pfldsc FROM seguranca.perfil  WHERE pflcod = {$pflcod}";
            $pfl = $modelSegurancaPerfilUsuario->recuperar($sql);
            $sql = "
                    SELECT pfldsc,*,usunome FROM seguranca.perfil pfl
                    INNER JOIN seguranca.perfilusuario pfu ON pfu.pflcod = pfl.pflcod
                    INNER JOIN seguranca.usuario usu ON pfu.usucpf = usu.usucpf
                    WHERE pfu.usucpf = '{$_SESSION['usucpf']}' AND pfl.sisid = 231
                    ORDER BY pfl.pflnivel LIMIT 1";
            $dadosPerfilUsuarioSessao = $modelSegurancaHistoricoUsuario->recuperar($sql);

            $historicoUsuario = array('usucpf' => $_SESSION['usucpf'], 'suscod' => 'A', 'sisid'  => 231,
                'htudsc' =>
                    "Usuário incluído pelo {$dadosPerfilUsuarioSessao['pfldsc']} {$dadosPerfilUsuarioSessao['usunome']} através do formulário Configurações PAR
                     do sistema PAR com o perfil {$pfl['pfldsc']}",
                ''
            );
            $modelSegurancaHistoricoUsuario->popularDadosObjeto($historicoUsuario);
            $modelSegurancaHistoricoUsuario->salvar();

            /* Grava Usuário responsabilidade*/
            if($dadosUnidade['itrid'] == 2){
            	$muncodResp    = $dadosUnidade['muncod'];
            	$estufResp     = null;
            }else{
            	$muncodResp    = null;
            	$estufResp     = $dadosUnidade['estuf'];
            }
            $arrResp = $modelUsuarioRepsonsabilidade->retornaUsuarioResponsabilidade($arrPost['entcpf'], $pflcod, $muncodResp, $estufResp);

            $arrDadosResp = array();
            $arrDadosResp['rpuid']  = $arrResp['rpuid'];
            $arrDadosResp['pflcod'] = $pflcod;
            $arrDadosResp['usucpf'] = $arrPost['entcpf'];
            $arrDadosResp['muncod'] = $muncodResp;
            $arrDadosResp['estuf']  = $estufResp;

            $modelUsuarioRepsonsabilidade->popularDadosObjeto($arrDadosResp);
            $modelUsuarioRepsonsabilidade->salvar();
            /* FIM - Grava Usuário responsabilidade*/

            $modelInstrumentoUnidadeEntidade->commit();
            $modelSegurancaUsuario->commit();
            $modelSegurancaUsuarioSistema->commit();
            $modelSegurancaHistoricoUsuario->commit();
            $modelUsuarioRepsonsabilidade->commit();

            simec_redirecionar($url, 'success',$retornoEmail);
        } catch(Exception $e) {

            $modelInstrumentoUnidadeEntidade->rollback();
            $modelSegurancaUsuario->rollback();
            $modelSegurancaUsuarioSistema->rollback();
            $modelSegurancaPerfilUsuario->rollback();
            $modelSegurancaHistoricoUsuario->rollback();
            simec_redirecionar($url, 'error', $e->getMessage());
        }

    }//end salvar()

    /**
     * Função verificaPreenchimentoEquipeTecnica
     * - verifica o preenchimento dos dados dos membros da equipe técnica e retorna um percentual
     * de preenchimento.
     *
     * @return percentual de preenchimento.
     *
     */
    public function verificaPreenchimentoEquipeTecnica($inuid)
    {
        $equipeTecnica = new Par3_Model_InstrumentoUnidadeEntidade();
        $itrid = Par3_Controller_InstrumentoUnidade::pegarItrid($inuid);

        $tcpid = ($itrid == 1) ? Par3_Model_TipoConfiguracoesPar::LIMITE_EQUIPE_ESTADUAL : Par3_Model_TipoConfiguracoesPar::LIMITE_EQUIPE_MUNICIPAL;

        $tenid = Par3_Model_InstrumentoUnidadeEntidade::EQUIPE_TECNICA;
        $arrDados['inuid'] = $inuid;
        $arrDados['tenid'] = $tenid;
        $arrMembrosEquipeTecnica = $equipeTecnica->carregarListaEntPorTipo($arrDados);
        $mConfiguracaoParUnidade = new Par3_Controller_ConfiguracaoParUnidadeControle();
        $mconfiguracaoGlobal     = new Par3_Model_ConfiguracoesPar();

        $qtdLimiteConfiguracao = $mConfiguracaoParUnidade->retornaConfiguracaoUnidadeLimite($inuid,$tcpid);
        $qtdLimiteGlobal = $mconfiguracaoGlobal->retornaConfiguracaoGlobal($tcpid);


        $qtdLimite = $qtdLimiteConfiguracao == 0 || !$qtdLimiteConfiguracao? $qtdLimiteGlobal['copvalor']:$qtdLimiteConfiguracao;

        //retorna a quantidade de membros da Equipe Técnica
        $qtdMembrosEquipe = $arrMembrosEquipeTecnica?count($arrMembrosEquipeTecnica):0;

        $perc   = 0;
        $campos = 2;

        if($qtdMembrosEquipe > 0 && $qtdMembrosEquipe <= $qtdLimite){
            foreach($arrMembrosEquipeTecnica as $membroEquipe){
                if($membroEquipe['entcpf'] != '') $perc++;
                if($membroEquipe['entnome'] != '') $perc++;
            }

            $perc = ($perc / $campos * 100) / $qtdMembrosEquipe;
        }

        if($qtdMembrosEquipe <= 0){
            $perc = 0;
        }

        if($qtdMembrosEquipe > $qtdLimite){
            $perc = 0;
        }
        return (int)$perc;
    }//end verificaPreenchimentoEquipeTecnica();

    /**
     * Função invativar
     * - inativa membro da equipe técnica.
     *
     * @return void.
     */
    public function invativar($arrPost)
    {
        $modelSegurancaUsuario            = new Seguranca_Model_Usuario();
        $modelSegurancaUsuarioSistema     = new Seguranca_Model_Usuario_sistema();
        $modelInstrumentoUnidadeEntidade  = new Par3_Model_InstrumentoUnidadeEntidade($arrPost['entid']);
        $modelSegurancaPerfilUsuario      = new Seguranca_Model_Perfilusuario();
        $modelSegurancaHistoricoUsuario   = new Seguranca_Model_Historicousuario();


        $url = "par3.php?modulo=principal/planoTrabalho/dadosUnidade&acao=A&inuid={$arrPost['inuid']}&menu=tecnico";
        try {
            //inativa usuário em instrumentounidade_entidade
            $modelInstrumentoUnidadeEntidade->inativaEntidadesInuidPorId($arrPost['inuid'],$arrPost['entid']);
            $modelInstrumentoUnidadeEntidade->commit();

            $sql = "SELECT * FROM par3.instrumentounidade_entidade WHERE entid = {$arrPost['entid']}";
            $instrumentoUniEnt = $modelInstrumentoUnidadeEntidade->recuperar($sql);
            $modelInstrumentoUnidadeEntidade->commit();

            //verificar se há cpf cadastrado com o perfil estadual ou municipal
            $pflcod = $arrPost['tcpid'] == 1 ? 1431:1432;
            //Deletar vinculo de usuário com perfil
            $sql = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '{$instrumentoUniEnt['entcpf']}' AND pflcod = {$pflcod}";
            $modelSegurancaPerfilUsuario->executar($sql);
            $modelSegurancaPerfilUsuario->commit();

            //Verificar se há perfil do usuário vinculado ao sistema 231(PAR)
            $perfilUsuario = "
                    SELECT * FROM seguranca.perfil pfl
                    INNER JOIN seguranca.perfilusuario pflusu ON pfl.pflcod = pflusu.pflcod
                    WHERE pflusu.usucpf = '{$arrPost['entcpf']}' AND pfl.sisid = 231;";
            $modelSegurancaUsuarioSistema->recuperar($sql);
            $modelSegurancaUsuarioSistema->commit();

            if(!$perfilUsuario){//se não houver o usuário é bloqueado no sistema 231(PAR)
                $arrSegUsuario = array('usucpf' => $instrumentoUniEnt['entcpf'],'sisid' => 231,'susstatus' => 'B','suscod' => 'B');
                $modelSegurancaUsuarioSistema->popularDadosObjeto($arrSegUsuario);
                $modelSegurancaUsuarioSistema->salvar();
                $modelSegurancaUsuarioSistema->commit();
            }

            //Se não estiver ativo em segurança.usuario_sistema bloquear em seguranca.usuario
            $sql = "SELECT * FROM seguranca.usuario_sistema WHERE usucpf = '{$instrumentoUniEnt['entcpf']}' AND sisid = 231";
            $usuarioSistema = $modelSegurancaUsuarioSistema->recuperar($sql);
            $modelSegurancaUsuarioSistema->commit();

            if(!$instrumentoUniEnt['entcpf']){
                $usuario = array('usucpf' => $instrumentoUniEnt['entcpf'],'suscod' => 'B');
                $modelSegurancaUsuario->popularDadosObjeto($usuario);
                $modelSegurancaUsuario->salvar();
                $modelSegurancaUsuario->commit();
            }

            //gravar no histórico
            $sql = "SELECT pfldsc FROM seguranca.perfil  WHERE pflcod = {$pflcod}";
            $pfl = $modelSegurancaPerfilUsuario->recuperar($sql);
            $sql = "
                    SELECT pfldsc,*,usunome FROM seguranca.perfil pfl
                    INNER JOIN seguranca.perfilusuario pfu ON pfu.pflcod = pfl.pflcod
                    INNER JOIN seguranca.usuario usu ON pfu.usucpf = usu.usucpf
                    WHERE pfu.usucpf = '{$_SESSION['usucpf']}' AND pfl.sisid = 231
                    ORDER BY pfl.pflnivel LIMIT 1";
            $dadosPerfilUsuarioSessao = $modelSegurancaHistoricoUsuario->recuperar($sql);
            $historicoUsuario = array('usucpf' => $_SESSION['usucpf'], 'suscod' => 'B', 'sisid'  => 231,
                'htudsc' =>
                    "Usuário Bloqueado pelo {$dadosPerfilUsuarioSessao['pfldsc']} {$dadosPerfilUsuarioSessao['usunome']} através do formulário Configurações PAR
                     do sistema PAR com o perfil {$pfl['pfldsc']}",
            );
            $modelSegurancaHistoricoUsuario->popularDadosObjeto($historicoUsuario);
            $modelSegurancaHistoricoUsuario->salvar();
            $modelSegurancaHistoricoUsuario->commit();

            simec_redirecionar($url, 'success');
            die();
        } catch(Exception $e) {
            $modelSegurancaUsuario->rollback();
            $modelSegurancaUsuarioSistema->rollback();
            $modelSegurancaPerfilUsuario->rollback();
            $modelSegurancaHistoricoUsuario->rollback();
            simec_redirecionar($url, 'error');
            die();
        }

    }//end salvar()

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


    public function recuperarIntegranteEquipeTecnica($arrPost)
    {
        $instrumentoUnidadeEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $arrEquipeTecnica = $instrumentoUnidadeEntidade->recuperarIntegranteEquipeTecnica($arrPost);
        $arrEquipeTecnica['entcpf'] = formatar_cpf($arrEquipeTecnica['entcpf']);
        return $arrEquipeTecnica;
    }


    /**
     * Validar se o se o limite de integrantes  foi atingido
     * de acordo com o que foi cadastrado na tabela par3.configuracaopar_unidade
     * @param $array['inuid','tcpid]
     * @return boolean.
     */
    public function limiteIntegrantesExcedido($arrPost)
    {
        $controllerConfiguracaoParUnidade = new Par3_Controller_ConfiguracaoParUnidadeControle();
        $modelInstrumentoUnidadeEntidade  = new Par3_Model_InstrumentoUnidadeEntidade();
        $modelConfiguracoesPar            = new Par3_Model_ConfiguracoesPar();

        $limiteGlobal = $modelConfiguracoesPar->retornaConfiguracaoGlobal($arrPost['tcpid']);
        $couvalor     = $controllerConfiguracaoParUnidade->retornaConfiguracaoUnidadeLimite($arrPost['inuid'],$arrPost['tcpid']);
        $qtdEntidades = $this->contarMembrosEquipeTecnica($arrPost['inuid']);

        $qtdLimite = $couvalor == 0 || !$couvalor ? $limiteGlobal['copvalor']:$couvalor;

        if($qtdEntidades >= $qtdLimite){
            return true;
        }
    }

    public function contarMembrosEquipeTecnica($inuid)
    {
        $mInstrumentoEntidade = new Par3_Model_InstrumentoUnidadeEntidade();
        $sql =  $mInstrumentoEntidade->contarEntidadesSQL($inuid,15,'A');
        $arrCount =  $mInstrumentoEntidade->recuperar($sql);
        return $arrCount['count'];
    }

    /**
     * Enviar email com login e senha para um novo integrante cadastrado na Equipe Técnica
     * @param $array['entnome','unidade','entemail','senha','mailto', 'assunto', 'mensagem']
     * @return boolean.
     */
    public function enviarEmailNovoIntegrante($dadosEmail)
    {
        $strEmailTo  = $dadosEmail['mailto'];
        $strAssunto  = $dadosEmail['assunto'];

            $strMensagem =
                "
                <pre align=\"center\" style=\"text-align: justify;\"  >

               Prezado(a) Sr(a). {$dadosEmail['entnome']},

               Você foi cadastrado no SIMEC, sistema PAR, com o perfil de Equipe Técnica do Município/Estado de {$dadosEmail['unidade']}

               Dados de Acesso:

                Login:<b>{$dadosEmail['entcpf']}</b>
                Senha:<b>{$dadosEmail['senha']}</b>


                Esta é uma mensagem automática e não é necessária nenhuma resposta ou confirmação.

                <b> Ministério da Educação </b>

                </pre>";

        $remetente = array("nome"=>"SIMEC", "email"=>"noreply@mec.gov.br");

        $strMensagem = html_entity_decode($strMensagem);

        $retorno = enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem);

        return $retorno;
    }
}//end class

?>