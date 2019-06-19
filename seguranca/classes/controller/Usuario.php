<?php
/**
 * Classe de controle do  usuario
 * @category Class
 * @package  A1
 * @version $Id$
 * @author   VICTOR MARTINS MACHADO <victormachado@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-02-2017
 * @link     no link
 */



/**
 * Seguranca_Controller_Usuario
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 03-02-2017
 * @link     no link
 */

include_once APPRAIZ . "seguranca/classes/model/Usuario.php";
include_once APPRAIZ . "seguranca/classes/model/Perfilusuario.inc";
include_once APPRAIZ . "seguranca/classes/model/Usuario_sistema.php";

class Seguranca_Controller_Usuario
{
    private $model;

    public function __construct($usucpf = null){
        $this->setModel($usucpf);
    }

    /**
     * Instancia a model de usuário, carregando os dados do usuário informado.
     *
     * @param null $usucpf
     * @return $this
     */
    public function setModel($usucpf = null){
        $this->model = new Seguranca_Model_Usuario($usucpf);
        return $this;
    }

    /**
     * Retorna a model carregada.
     *
     * @return Seguranca_Model_Usuario
     */
    public function getModel(){
        return $this->model;
    }
	/**
	 *
	 * @param unknown $arrUsuario*/
    public function inativaAcessoUsuariosAntigos( $arrParams ){

    	return $this->model->inativaPerfisIndevidos( $arrParams );

    }

    public function insereVinculosFalhos(){
    	
    	return $this->model->insereVinculosFalhos();
    }
    public function listaPendenciasVinculo(){
    	
    	$dados = $this->model->listaPendenciasVinculo();
    	
    	if(count($dados) > 0 )
    	{
    		foreach($dados as $k => $v )
    		{
    			$relatorio .= implode('-->', $v) . '<br>';
    		}
    	
    	}
    	else
    	{
    		$relatorio = "";
    	}
    	
    	return $relatorio;
    }
    
    public function corrigeSenhasErradas(  )
    {
    	return $this->model->corrigeSenhasErradas();
    	
    }
    
    public function atualizaDadosUsuario( $arrUsuario ){
        $cpfadm = '00000000191';
        $usucpf = $arrUsuario['entcpf'];
        $senha = 'ent' . substr($arrUsuario['entcpf'], -4); // -- SENHA PADRÃO
        $retorno = [];

        /** ARRAY COM OS PERFIS DE ACESSO */
        $tpentidade = [
            231 => [
                '02' => PAR3_PERFIL_PREFEITO, /*PREFEITO*/
                '10' => PAR3_PERFIL_EQUIPE_ESTADUAL_SECRETARIO, /*SECRETARIO ESTADUAL*/
                '28' => PAR3_PERFIL_DIRIGENTE_MUNICIPAL /*DIRIGENTE MUNICIPAL*/
            ],
            23 => [
                '02' => PAR_PERFIL_PREFEITO,
                '10' => PAR_PERFIL_EQUIPE_ESTADUAL_SECRETARIO,
                '28' => PAR_PERFIL_EQUIPE_ESTADUAL_APROVACAO
            ]
        ];

        foreach ($tpentidade as $sisid => $perfis) {
            /** PERFIL DO USUÁRIO NO SISTMA INFORMADO */
            $pflcod = $perfis[(string)$arrUsuario['co_tp_entidade']];

            /** VERIFICA SE O USUÁRIO JÁ ESTÁ CADASTRADO NO SIMEC */
            $exists = $this->model->exists($usucpf);
            if($exists === true){
                // -- Usuário já cadastrado no SIMEC

                /** CARREGA OS DADOS DO USUÁRIO NA MODEL */
                $this->setModel($usucpf);
				
                //
                $emailUsuario	= $this->model->usuemail;
                $chaveAtivacao	= $this->model->usuchaveativacao;
                $emailDaRotina	= $arrUsuario['entemail'];
                
                $trocaEmail = false;
                $resetaSenha = false;  
                if( $emailUsuario != $emailDaRotina )
                {
                	
                	$trocaEmail = true;
                	if($chaveAtivacao != 't')
                	{
                		$resetaSenha = true;
                	}
                	else
                	{
                		$resetaSenha = false;
                	}
                }
                
                /** RETORNA O STATUS GERAL DO USUÁRIO */
                $statusGeral 	= $this->model->retornaStatusGeral();
                /** VERIFICA SE O USUÁRIO ESTÁ BLOQUEADO */
                if (($statusGeral === 'B' || $statusGeral === 'P')) {
                    $this->model->ususenha          = md5_encrypt_senha($senha, '');
                    $this->model->usuchaveativacao  = 'FALSE';
                    $this->model->suscod 			= 'A';
                    $this->model->salvar();
                    $this->model->commit();
                    $retorno = [
                        'error'    => false,
                        'usucpf'   => $usucpf,
                        'ususenha' => $senha
                    ];
                }

                /** RETORNA O STATUS GERAL DO USUÁRIO */
                $statusSistema 	= $this->model->retornaStatusSistema($sisid);
                if(!empty($statusSistema)) {
                    /** VERIFICA SE O USUÁRIO ESTÁ BLOQUEADO */
                    if ($statusSistema === 'B' || $statusSistema === 'P') {
                        if ($this->model->alteraStatusGeral($sisid, 'A', 'Usuário ativado via integração SAPE/SIMEC', $cpfadm, $usucpf)) {
                            $retorno = [
                            'error'    => false
                            ];
                        } else {
                            $retorno = [
                            'error'    => true,
                            'mensagem'      => 'Erro ao alterar o status do usuário.'
                            ];
                        }
                    } 
                        /** ATRIBUI O PERFIL INDICADO PELO CAMPO co_tp_entidade AO USUÁRIO */
                        $pfl = new Seguranca_Model_Perfilusuario();

                        $pfl->atualizaPerfis($usucpf, [$pflcod]);

                        $retorno = [
                        'error'    => false
                        ];
                    
                } else {
                    /** LIBERA AO USUÁRIO O ACESSO AO SISTEMA INFORMADO */
                    $uss = new Seguranca_Model_Usuario_sistema();
                    $uss->usucpf    = $usucpf;
                    $uss->sisid     = $sisid;
                    $uss->pflcod    = $pflcod;
                    $uss->suscod    = 'A';
                    $uss->inserir();

                    /** ATRIBUI O PERFIL INDICADO PELO CAMPO co_tp_entidade AO USUÁRIO */
                    $pfl = new Seguranca_Model_Perfilusuario();
                    $pfl->removerPerfis($usucpf, [$pflcod])
                        ->salvarDados([
                                'usucpf' => $usucpf,
                                'pflcod' => $pflcod
                            ]);

                    $retorno = [
                        'error'    => false,
                        'usucpf'   => $usucpf,
                        'ususenha' => $senha
                    ];
                }
                
                if( $trocaEmail )
                {
                	if($resetaSenha)
                	{
                		$this->model->ususenha          = md5_encrypt_senha($senha, '');
                		$this->model->usuchaveativacao  = 'FALSE';
                		$this->model->usuemail  		= $emailDaRotina;
                		$this->model->salvar();
                		$this->model->commit();
                		$retorno = [
                				'error'    => false,
                				'usucpf'   => $usucpf,
                				'ususenha' => $senha
                		];
                	}
                	else
                	{
                		$resetaSenha = false;
                		$this->model->usuemail  		= $emailDaRotina;
                		$this->model->salvar();
                		$this->model->commit();
                		$retorno = [
                				'error'    => false
                		];
                	}
                }
            } else {
                // -- Usuário não existe
                if(empty($arrUsuario['entnome']) || empty($arrUsuario['entemail']) || empty($usucpf)){
                    $retorno = [
                        'error'    => true,
                        'mensagem'  => 'Informações insuficientes para o cadastro do usuário',
                        'usucpf'   => $usucpf,
                        'usunome'  => $arrUsuario['entnome'],
                        'usuemail' => $arrUsuario['entemail']
                    ];

        			return $retorno;
                } else {

                    /** POPULA A MODEL COM OS DADOS DA ENTIDADE E SALVA NO BANCO DE DADOS */
                    $this->model->popularDadosObjeto([
                        'usucpf' => $usucpf,
                        'usunome' => $arrUsuario['entnome'],
                        'usuemail' => $arrUsuario['entemail'],
                        'ususenha' => md5_encrypt_senha($senha, ''),
                        'usuchaveativacao' => 'FALSE',
                        'ususexo' => str_to_upper($arrUsuario['entsexo']),
                        'regcod' => $arrUsuario['estuf'],
                        'muncod' => $arrUsuario['muncod'],
                        'usufoneddd' => $arrUsuario['entnumdddcomercial'],
                        'usufonenum' => $arrUsuario['entnumcomercial']
                    ]);
                    $this->model->salvar();
                    $test = $this->model->commit();

                    if($test){
	                    /** INSTANCIA A MODEL DA TABELA USUARIO_SISTEMA E INSERE O REGISTRO QUE ATIVA O USUÁRIO NO SISTEMA */
	                    $uss = new Seguranca_Model_Usuario_sistema();
	                    $uss->usucpf = $usucpf;
	                    $uss->sisid = $sisid;
	                    $uss->pflcod = $pflcod;
	                    $uss->suscod = 'A';
	                    $uss->inserir();

	                    /** ATRIBUI O PERFIL INDICADO PELO CAMPO co_tp_entidade AO USUÁRIO */
	                    $pfl = new Seguranca_Model_Perfilusuario();
	                    $pfl->salvarDados([
	                        'usucpf' => $usucpf,
	                        'pflcod' => $pflcod
	                    ]);

	                    $retorno = [
	                        'error' => false,
	                        'usucpf' => $usucpf,
	                        'ususenha' => $senha
	                    ];
                    }
                }
            }


        }

        return $retorno;
    }

    /**
     * Função gravar
     * - grava os dados
     *
     */
    public function salvar()
    {
        global $url;
//        $this->model = new Seguranca_Controller_Usuario();
        $this->model->popularDadosObjeto();
        $url .= '&usucpf=' . $this->model->usucpf;

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
        $id = $_GET['usucpf'];
        $url = "aspar.php?modulo=principal/proposicao/index&acao=A&usucpf={$id}";
        $usuario = new Seguranca_Model_Usuario($id);
        try{
             $usuario->Xstatus = 'I';
             $usuario->Xdtinativacao = date('Y-m-d H:i:s');
             $usuario->Xusucpfinativacao = $_SESSION['usucpf'];

             $usuario->salvar();
             $usuario->commit();
            simec_redirecionar($url, 'success');
        } catch (Simec_Db_Exception $e) {
            simec_redirecionar($url, 'error');
        }
    }

    public function atualizaDadosUsuarioCacs($arrConselheiro, $pflcod, $sisid, $rotinaCacs = true){
        $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
        $usucpf = $arrConselheiro['usucpf'];
        $senha = $arrConselheiro['ususenha'];
        $cpfadm = '00000000191';
        $retorno = [];
        $enviaEmail = false;

        $exists = $this->model->exists($usucpf);
        if($exists === true){
            // -- Usuário já cadastrado no SIMEC

            /** CARREGA OS DADOS DO USUÁRIO NA MODEL */
            $this->setModel($usucpf);

            //
            $emailUsuario	= $this->model->usuemail;
            $chaveAtivacao	= $this->model->usuchaveativacao;
            $emailDaRotina	= $arrConselheiro['email_conselheiro'][0];

            $trocaEmail = false;
            $resetaSenha = false;
            if( $emailUsuario != $emailDaRotina )
            {

                $trocaEmail = true;
                if($chaveAtivacao != 't')
                {
                    $resetaSenha = true;
                }
                else
                {
                    $resetaSenha = false;
                }
            }

            if(!$rotinaCacs){
                $trocaEmail = true;
                $resetaSenha = true;
            }

            /** RETORNA O STATUS GERAL DO USUÁRIO */
            $statusGeral 	= $this->model->retornaStatusGeral();
            /** VERIFICA SE O USUÁRIO ESTÁ BLOQUEADO */
            if (($statusGeral === 'B' || $statusGeral === 'P')) {
                $this->model->ususenha          = md5_encrypt_senha($senha, '');
                $this->model->usuchaveativacao  = 'FALSE';
                $this->model->suscod 			= 'A';
                $this->model->salvar();
                $this->model->commit();
                $retorno = [
                    'error'    => false,
                    'usucpf'   => $usucpf,
                    'ususenha' => $senha
                ];
                $enviaEmail = true;
            }

            /** RETORNA O STATUS GERAL DO USUÁRIO */
            $statusSistema 	= $this->model->retornaStatusSistema($sisid);
            if(!empty($statusSistema)) {
                /** VERIFICA SE O USUÁRIO ESTÁ BLOQUEADO */
                if ($statusSistema === 'B' || $statusSistema === 'P') {
                    if ($this->model->alteraStatusGeral($sisid, 'A', 'Usuário ativado via rotina do CACS', $cpfadm, $usucpf)) {
                        $retorno = [
                            'error'    => false
                        ];
                    } else {
                        $retorno = [
                            'error'    => true,
                            'mensagem'      => 'Erro ao alterar o status do usuário.'
                        ];
                    }
                }
                /** ATRIBUI O PERFIL INDICADO PELO CAMPO ds_funcao AO USUÁRIO */
                $pfl = new Seguranca_Model_Perfilusuario();
                $pfl->atualizaPerfisCacs($usucpf, $pflcod, $sisid);


                $retorno = [
                    'error'    => false
                ];

            } else {
                /** LIBERA AO USUÁRIO O ACESSO AO SISTEMA INFORMADO */
                $uss = new Seguranca_Model_Usuario_sistema();
                $uss->usucpf    = $usucpf;
                $uss->sisid     = $sisid;
                $uss->pflcod    = $pflcod[0];
                $uss->suscod    = 'A';
                $uss->inserir();

                /** ATRIBUI O PERFIL INDICADO PELO CAMPO ds_funcao AO USUÁRIO */
                $pfl = new Seguranca_Model_Perfilusuario();
                $pfl->atualizaPerfisCacs($usucpf, $pflcod, $sisid);


                $retorno = [
                    'error'    => false,
                    'usucpf'   => $usucpf,
                    'ususenha' => $senha
                ];
            }

            if( $trocaEmail )
            {
                if($resetaSenha)
                {
                    $this->model->ususenha          = md5_encrypt_senha($senha, '');
                    $this->model->usuchaveativacao  = 'FALSE';
                    $this->model->usuemail  		= $emailDaRotina;
                    $this->model->salvar();
                    $this->model->commit();
                    $retorno = [
                        'error'    => false,
                        'usucpf'   => $usucpf,
                        'ususenha' => $senha
                    ];
                    $enviaEmail = true;
                }
                else
                {
                    $resetaSenha = false;
                    $this->model->usuemail  		= $emailDaRotina;
                    $this->model->salvar();
                    $this->model->commit();
                    $retorno = [
                        'error'    => false
                    ];
                }
            }
        }

        /** SALVAR USUARIO RESPONSABILIDADE */
        if($sisid == PAR3_SIS_ID){
            $usuarioResponsabilidade->salvarUsuarioResponsabilidade($arrConselheiro, $pflcod);
        }
        else {
            $usuarioResponsabilidade->salvarUsuarioResponsabilidade($arrConselheiro, $pflcod, 'par.usuarioresponsabilidade');
        }
        $usuarioResponsabilidade->commit();
        if($enviaEmail){
            if (IS_PRODUCAO) {
                $modelCacs = new Par3_Model_CACS();
                $modelCacs->enviarEmailCacs($arrConselheiro);
            }
        }
        return $retorno;
    }

}
?>