<?php

include_once APPRAIZ . "includes/classes/modelo/seguranca/Usuario.class.inc";

class Par3_Model_CACS extends Modelo {

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "integracao.cacs";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array();

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'co_mun_ibge_conselho' => null,
        'esf_adm_conselho' => null,
        'sit_mandato' => null,
        'uf_conselho' => null,
        'mun_conselho' => null,
        'email_conselho' => null,
        'cpf_conselheiro' => null,
        'no_conselheiro' => null,
        'email_conselheiro' => null,
        'sit_conselheiro' => null,
        'ds_segmento' => null,
        'tp_membro' => null,
        'ds_funcao' => null
    );

    public function listarConselheiros($valor, $inuid) {
        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);
        $itrid = $modelInstrumentoUnidade->itrid;

        if ($itrid === '1' || $itrid === '3') {
            $campo = "uf_conselho";
            $tipoEsfera = "ESTADUAL";
        } elseif ($itrid == '2') {
            $campo = "co_mun_ibge_conselho";
            $tipoEsfera = "MUNICIPAL";
        } else {
            $campo = "uf_conselho";
            $tipoEsfera = "FEDERAL";
        }
        $sql = "SELECT *
			    FROM {$this->stNomeTabela}
			    WHERE $campo = '{$valor}' AND esf_adm_conselho = '{$tipoEsfera}'
                ORDER BY
                CASE
                  WHEN ds_funcao = 'PRESIDENTE' THEN 1
                  WHEN ds_funcao = 'MEMBRO' THEN 3
                  ELSE 2
                END ASC";

        $dados = adapterConnection::coorporativo()->carregar($sql, 0, 60);
        adapterConnection::coorporativo()->getDb()->close();

        return $dados;
    }

    public function verificaPreenchimentoConselheiros($itrid, $inuid) {

        $modelInstrumentoUnidade = new Par3_Model_InstrumentoUnidade($inuid);

        $itrid = $modelInstrumentoUnidade->itrid;

        if ($itrid === '1' || $itrid === '3')
            $value = $modelInstrumentoUnidade->estuf;
        elseif ($itrid === '2')
            $value = $modelInstrumentoUnidade->muncod;
        else
            $value = $modelInstrumentoUnidade->estuf;


        $conselheiros = $this->listarConselheiros($value, $inuid);

        $perc = 0;

        if (is_array($conselheiros) && count($conselheiros) > 0) {
            $perc++;
        }

        if ($perc > 0)
            $perc = $perc / 1 * 100;

        if ($conselheiros[0]['sit_conselheiro'] != "REGULAR") {
            $perc = 0;
        }

        return (int) $perc;
    }

    private function listarTodosConselheiros($limite, $offset) {
        $sql = "SELECT * FROM {$this->stNomeTabela} OFFSET {$offset} LIMIT {$limite}";
        $dados = adapterConnection::coorporativo()->carregar($sql);
        adapterConnection::coorporativo()->getDb()->close();
        return $dados;
    }

    public function atualizarTabelaConselheiro($limite, $offset) {


        $cons = $this->listarTodosConselheiros($limite, $offset);

//        if($offset == 0) {
//            $this->executar('TRUNCATE par3.cacs');
//            $this->executar('TRUNCATE par.log_carga_cacs');
//            $this->commit();
//        }

        if (count($cons) == 0) {
            return false;
        }

        if (is_array($cons)) {
            try {

                foreach ($cons as $key => $value) {
                    $no_conselheiro = addslashes($value['no_conselheiro']);

                    $value = $this->validaDadosConselheiro($value);

                    if ($value) {
                        $existeConselheiro = $this->pegaUm("SELECT cpf_conselheiro FROM par3.cacs WHERE cpf_conselheiro = '{$value['cpf_conselheiro']}'");

                        if (!$existeConselheiro) {
                            $sql = "INSERT INTO par3.cacs(co_mun_ibge_conselho, esf_adm_conselho, sit_mandato, uf_conselho,
                                                mun_conselho, email_conselho, cpf_conselheiro, no_conselheiro,
                                                email_conselheiro, sit_conselheiro, ds_segmento, tp_membro,
                                                ds_funcao, dt_ult_carga)
                              VALUES (
                                  '{$value['co_mun_ibge_conselho']}', '{$value['esf_adm_conselho']}', '{$value['sit_mandato']}', '{$value['uf_conselho']}',
                                  '{$value['mun_conselho']}', '{$value['email_conselho']}', '{$value['cpf_conselheiro']}', '{$no_conselheiro}',
                                  '{$value['email_conselheiro']}', '{$value['sit_conselheiro']}', '{$value['ds_segmento']}', '{$value['tp_membro']}',
                                  '{$value['ds_funcao']}', '{$value['dt_ult_carga']}'
                              );";

                            $this->executar($sql);
                        }
                    }
                }

                if ($this->commit()) {
                    $sqlCount = "SELECT count(*) FROM par3.cacs";
                    $numRegistros = $this->pegaUM($sqlCount);
                    echo "Realizando carga na tabela par3.cacs - " . $numRegistros;
                    return true;
                };
            } catch (Exception $exc) {
                $this->rollback();
            }
        }
    }

    /*
     * @todo temporário 
     * */

    public function deletaJaInseridos() {
        $this->executar("
			DELETE FROM par3.cacs
			WHERE cpf_conselheiro in 
				(
					SELECT DISTINCT usu.usucpf 
					FROM 
						par.usuarioresponsabilidade ur
					INNER JOIN seguranca.usuario usu ON usu.usucpf = ur.usucpf AND suscod = 'A'
					where usu.usucpf in (  select DISTINCT cpf_conselheiro from par3.cacs ) 
					AND rpustatus = 'A'
					AND ur.pflcod in( 1504, 1503 )
				)
			");
        return $this->commit();
    }

    public function conselheirosQueNaoPossuiLogin($limite, $offset) {
        $sql = "
            SELECT count(1),
                cpf_conselheiro,
                to_json(array_agg(no_conselheiro)) AS no_conselheiro,
                to_json(array_agg(co_mun_ibge_conselho)) AS co_mun_ibge_conselho,
                to_json(array_agg(uf_conselho)) AS uf_conselho,
                to_json(array_agg(esf_adm_conselho)) AS esf_adm_conselho,
                to_json(array_agg(ds_funcao)) AS ds_funcao,
                to_json(array_agg(email_conselheiro)) AS email_conselheiro,
                substring(cpf_conselheiro from 1 for 3) || 'CACS' AS senha
            FROM par3.cacs
            WHERE cpf_conselheiro IN (
                SELECT cpf_conselheiro FROM par3.cacs
                EXCEPT
                SELECT usucpf FROM seguranca.usuario
                )

            AND email_conselheiro != ''
            AND trim(sit_mandato) = 'REGULAR'
            AND trim(sit_conselheiro) ILIKE 'REGULAR%'
            GROUP BY cpf_conselheiro OFFSET {$offset} LIMIT {$limite}
        ";
        $dados = $this->carregar($sql);
        return (!empty($dados) ? $dados : array());
    }

    private function popularUsuarioConselheiro($conselheiro) {
        $no_conselheiro = str_replace('"', '', str_replace('[', '', str_replace(']', '', $conselheiro['no_conselheiro'])));

        $no_conselheiro = explode(',', $no_conselheiro);
        $no_conselheiro = array_filter($no_conselheiro);
        $no_conselheiro = array_values($no_conselheiro);

        $co_mun_ibge_conselho = (json_decode($conselheiro['co_mun_ibge_conselho']));
        //$arrayEsfera = json_decode($conselheiro['esf_adm_conselho']);
        //$arrayEsfera = (is_array($arrayEsfera)) ? $arrayEsfera : Array();
//        $esf_adm_conselho = ($arrayEsfera);
        $esf_adm_conselho = ($conselheiro['esf_adm_conselho']);
        $uf_conselho = ($conselheiro['uf_conselho']);
        $ds_funcao = ($conselheiro['ds_funcao']);

        $email_conselheiro = json_decode($conselheiro['email_conselheiro']);
        if (is_null($email_conselheiro)) {
            $email_conselheiro_lista = str_replace('"', '', str_replace('[', '', str_replace(']', '', $conselheiro['email_conselheiro'])));
            $email_conselheiro = explode(',', $email_conselheiro_lista);
        }

        $dados = array();
        if (!empty($no_conselheiro)) {
            $dados['usucpf'] = $conselheiro['cpf_conselheiro'];
            $dados['usunome'] = $no_conselheiro;
            $dados['usuemail'] = $email_conselheiro;
            $dados['ususenha'] = $conselheiro['senha'];
            $dados['estuf'] = array($uf_conselho);
            $dados['muncod'] = array($co_mun_ibge_conselho);
//            $dados['esfera'] = $esf_adm_conselho;
            $dados['esfera'] = array($esf_adm_conselho);
            $dados['ds_funcao'] = $ds_funcao;
        }
        return $dados;
    }

    public function getPerfilCacs($perfis, $old = false) {
        $arr = array();
        foreach ($perfis as $perfil) {
            switch ($perfil) {
                case 'MEMBRO':
                    if ($old)
                        $arr[] = PAR_PERFIL_CONSELHEIRO;
                    else
                        $arr[] = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS;
                    break;
                case 'VICE-PRESIDENTE':
                    if ($old)
                        $arr[] = PAR_PERFIL_CONSELHEIRO;
                    else
                        $arr[] = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS;
                    break;
                case 'VICEPRESIDENTE':
                    if ($old)
                        $arr[] = PAR_PERFIL_CONSELHEIRO;
                    else
                        $arr[] = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS;
                    break;
                case 'PRESIDENTE':
                    if ($old)
                        $arr[] = PAR_PERFIL_PRESIDENTE;
                    else
                        $arr[] = Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS;
                    break;
            }
        }
        return $arr;
    }

    public function salvarUsuarioCacs($limite, $offset) {
//    	if($offset == 0)
//    	{
//
//    		$this->deletaJaInseridos();
//
//    	}
        //Busca lista de conselheiros que não tenham cadastro no simec
        $conselheirosNaoPossueLogin = $this->conselheirosQueNaoPossuiLogin($limite, $offset);

        //Busca conselheiros que tenham cadastro no simec mas não tenham usúario no PAR
        $conselheirosPossueLogin = $this->conselheirosQuePossuiLogin($limite, $offset);

        if ((count($conselheirosNaoPossueLogin) == 0) && (count($conselheirosPossueLogin) == 0)) {
            return false;
        }

        //Atualiza conselheiros que tenham cadastro no simec para terem acesso ao PAR e ao PAR3
        foreach ($conselheirosPossueLogin as $key => $conselheiro) {
            $usuarioArr = $this->popularUsuarioConselheiro($conselheiro);
            $pflcod = $this->getPerfilCacs($usuarioArr['ds_funcao']);
            $pflcodParAntigo = $this->getPerfilCacs($usuarioArr['ds_funcao'], true);

            $ctrlUsuario = new Seguranca_Controller_Usuario();
            $retornoPar = $ctrlUsuario->atualizaDadosUsuarioCacs($usuarioArr, $pflcodParAntigo, SISID_PAR_02);
            $retornoPar3 = $ctrlUsuario->atualizaDadosUsuarioCacs($usuarioArr, $pflcod, PAR3_SIS_ID);

            if ((!$retornoPar['error'])) {
                echo ($conselheiro['cpf_conselheiro'] . " - " . $conselheiro['email_conselheiro'] . 'Usúario atualizado PAR!') . "<br>";
            } else {
                echo ($conselheiro['cpf_conselheiro'] . " - " . $conselheiro['email_conselheiro'] . 'Erro ao atualizar PAR!') . "<br>";
            }
            if ((!$retornoPar3['error'])) {
                echo ($conselheiro['cpf_conselheiro'] . " - " . $conselheiro['email_conselheiro'] . 'Usúario atualizado PAR3!') . "<br>";
            } else {
                echo ($conselheiro['cpf_conselheiro'] . " - " . $conselheiro['email_conselheiro'] . 'Erro ao atualizar PAR3!') . "<br>";
            }
        }

        //Realiza cadastro no SIMEC dos conselheiros que não tem cadastro
        foreach ($conselheirosNaoPossueLogin as $key => $conselheiro) {

            $usuario = new Seguranca_Model_Seguranca_Usuario();
            $usuarioSistema1 = new Seguranca_Model_Seguranca_UsuarioSistema();
            $usuarioSistema2 = new Seguranca_Model_Seguranca_UsuarioSistema();
            $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
            $perfilUsuario = new Seguranca_Model_Seguranca_PerfilUsuario();

            $usuarioArr = $this->popularUsuarioConselheiro($conselheiro);

            $pflcod = $this->getPerfilCacs($usuarioArr['ds_funcao']);
            $pflcodParAntigo = $this->getPerfilCacs($usuarioArr['ds_funcao'], true);

            try {
                /** SALVAR USUARIO */
                $usuario->salvarUsuario($usuarioArr);

                $usuarioSistema1->salvarUsuarioSistema($conselheiro['cpf_conselheiro'], PAR3_SIS_ID);
                $usuarioSistema2->salvarUsuarioSistema($conselheiro['cpf_conselheiro'], SISID_PAR_02);

                /** SALVAR USUARIO RESPONSABILIDADE */
                $usuarioResponsabilidade->salvarUsuarioResponsabilidade($usuarioArr, $pflcod);
                $usuarioResponsabilidade->salvarUsuarioResponsabilidade($usuarioArr, $pflcodParAntigo, 'par.usuarioresponsabilidade');

                /** SALVAR USUARIO SISTEMA */
                $perfilUsuario->salvarPerfil($conselheiro['cpf_conselheiro'], $pflcod);
                $perfilUsuario->salvarPerfil($conselheiro['cpf_conselheiro'], $pflcodParAntigo);

                /** SALVAR PEFIL USUARIO */
                $usuario->commit();
                $usuarioSistema1->commit();
                $usuarioSistema2->commit();
                $usuarioResponsabilidade->commit();
                $perfilUsuario->commit();
                if (IS_PRODUCAO) {
                    $this->enviarEmailCacs($usuarioArr);
                }
                echo ($conselheiro['email_conselheiro'] . 'realizado!') . "<br>";
            } catch (Exception $exc) {
                $usuario->rollback();
                $usuarioSistema1->rollback();
                $usuarioSistema2->rollback();
                $perfilUsuario->rollback();
                echo $exc->getTraceAsString();
                return false;
            }
        }
        return true;
    }

    public function enviarEmailCacs($conselheiro) {
        setlocale(LC_ALL, 'pt_BR', 'pt_BR.utf-8', 'pt_BR.utf-8', 'portuguese');
        date_default_timezone_set('America/Sao_Paulo');
        $data = strftime('%d de %b de %Y');

        $cpf = $conselheiro['usucpf'] ? $conselheiro['usucpf'] : $conselheiro['cpf_conselheiro'];
        $senha = $conselheiro['ususenha'] ? $conselheiro['ususenha'] : $conselheiro['senha'];

        $email = str_replace("[", "", str_replace("]", "", $conselheiro['usuemail']));
        $remetente = array("nome" => "=?ISO-8859-1?B?" . base64_encode("PAR - Plano de Ações Articuladas") . "?=", "email" => "simecpar@mec.gov.br");
        $assuntoMensagemPadrao = "=?ISO-8859-1?B?" . base64_encode("Acesso ao SIMEC para consulta dos programas e projetos do PAR") . "?=";
        $mensagem = $this->getTextoEmail($cpf, $senha, $data);

        if($senha == ''){
            echo ('E-mail não enviado ' . $conselheiro['cpf_conselheiro']);
        } else{
            if (enviar_email($remetente, $email, $assuntoMensagemPadrao, $mensagem)) {
                echo ('E-mail enviado ' . $conselheiro['cpf_conselheiro']) ;
            } else {
                echo ('E-mail não enviado ' . $conselheiro['cpf_conselheiro']);
            }
        }
    }

    public function verificaPendenciaCacs($arrPerfil) {

        if (!is_array($arrPerfil)) {
            $arrPerfil = [$arrPerfil];
        }

        $flag = false;
        if (in_array(Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS, $arrPerfil)) {

            $flag = true;
            $modelCAE = new Seguranca_Model_Seguranca_Usuario();
            if ($objCAE = $modelCAE->carregarConselheiroCPF($_SESSION['usucpf'])) {
                if (empty($objCAE->entdtnascimento) ||
                        empty($objCAE->enttelefonecomercial) ||
                        empty($objCAE->enttelefonecomercial) ||
                        empty($objCAE->entemail) ||
                        empty($objCAE->entsexo) ||
                        empty($objCAE->cacfuncao) ||
                        empty($objCAE->cctid) ||
                        empty($objCAE->entnivelensino) ||
                        empty($objCAE->cacoutroconselho) ||
                        empty($objCAE->cacocupacao) ||
                        empty($objCAE->cacrenda)
                ) {
                    $flag = true;
                } else {
                    $flag = false;
                }
            }
        }

        return $flag;
    }

    protected function getUsuariosQueNaoSaoMaisConselheiros() {
        $pflCodConselheiro = Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS;
        $pflCodPresidente = Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS;

        $sql = " SELECT u.usucpf FROM seguranca.usuario u
                LEFT JOIN seguranca.perfilusuario pu ON pu.usucpf = u.usucpf
                WHERE u.suscod = 'A'
                      AND pu.pflcod IN ({$pflCodConselheiro}, {$pflCodPresidente} )
                      AND (SELECT COUNT(*) FROM seguranca.perfilusuario WHERE usucpf = u.usucpf) = 1
                EXCEPT
                SELECT cpf_conselheiro FROM par3.cacs
                ";
        $dados = $this->carregar($sql);
        return (!empty($dados) ? $dados : array());
    }

    public function bloquearUsuarioQueNaoSaoMaisConselheiro() {
        $usuarios = $this->getUsuariosQueNaoSaoMaisConselheiros();
        $modelUsuario = new Seguranca_Model_Seguranca_Usuario();
        $modelUsuario->inativarUsuarios($usuarios);
    }

    public function getTextoEmail($cpf, $senha, $data) {

        $texto = "
                <style type=\"text/css\">
                    p { direction: ltr; color: #000000; orphans: 2; widows: 2 }
                    p.western { font-family: \"Calibri\", sans-serif; font-size: 11pt; so-language: pt-BR }
                    p.cjk { font-family: \"Calibri\", sans-serif; font-size: 11pt }
                    p.ctl { font-family: \"Times New Roman\", serif; font-size: 11pt; so-language: ar-SA }
                    a:link { color: #0000ff }
                </style>
                <p class=\"western\" align=\"justify\" style=\"\">
                    <font face=\"Times New Roman, serif\">Prezados (as) Conselheiros (as),</font>
                </p>
                <p class=\"western\" align=\"justify\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\">
                        No intuito de apoiar o Conselho
                        de Acompanhamento e Controle Social do Fundeb (CACS-Fundeb), no
                        acompanhamento, controle social e análise de prestação de contas
                        dos recursos repassados no âmbito do Plano de Ações Articuladas
                        (PAR), conforme estabelecido no Decreto nº 6.094, de 24/04/2007 e na
                        Lei nº 12.695, de 25/07/2012, enviamos a seguir informações de
                        acesso ao Sistema Integrado de Monitoramento do Ministério da
                        Educação (SIMEC):
                    </font>
                </p>
                <p class=\"western\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\"><b>Endereço: simec.mec.gov.br</b></font>
                </p>
                <p class=\"western\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\"><b>Login: $cpf</b></font>
                </p>
                <p class=\"western\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\"><b>Senha: $senha</b></font>
                </p>
                <p class=\"western\" align=\"justify\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\">
                        Na busca em aperfeiçoar o
                        diálogo com estados e municípios, aumentando a transparência no
                        atendimento aos programas e consolidando as parcerias, o Plano espera
                        contar com a valiosa atuação dos atores envolvidos no controle
                        social, para o acompanhamento das ações educacionais pactuadas em
                        suas redes de ensino.
                    </font>
                </p>
                <p class=\"western\" align=\"justify\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\">
                        Para isso, inicialmente,
                        disponibiliza acesso à consulta das informações e ao módulo de
                        execução e acompanhamento do 2º ciclo (2011 a 2014).
                        Posteriormente, comunicaremos sobre a disponibilização do sistema
                        para a validação das ações executadas no PAR (2011/2014), pelo
                        Presidente do CACs-Fundeb.
                    </font>
                </p>
                <p class=\"western\" align=\"justify\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\">
                       No intuito de subsidiar o acompanhamento e análise das prestações de contas, no Módulo PAR 2011-2014, elaboramos um manual com orientações gerais, o qual será enviado oportunamente por meio eletrônico, assim como estará disponível no portal do FNDE e no Simec.
                    </font>
                </p>
                <p class=\"western\" align=\"justify\" style=\"text-indent: 1.25cm;\">
                    <font face=\"Times New Roman, serif\">Atenciosamente, </font>
                </p>
                <p class=\"western\" align=\"right\" style=\"\">
                    <font face=\"Times New Roman, serif\">Brasília, $data.</font></p>
                <p class=\"western\" align=\"right\" style=\"\">
                    <font face=\"Times New Roman, serif\"><b>Ministério da Educação</b></font></p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\"><br/>

                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\"><br/>

                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\">
                    <font face=\"Times New Roman, serif\">
                        Contatos
                        disponíveis para orientações e esclarecimento de dúvidas:
                    </font>
                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\"><br/>
                  &nbsp;
                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\">
                    <font face=\"Times New Roman, serif\">Central de Atendimento ao Cidadão (0800-616161)</font>
                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\"><br/>
                    &nbsp;
                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\">
                    <font face=\"Times New Roman, serif\">No FNDE:</font>
                </p>
                <p class=\"western\" style=\"margin-bottom: 0cm;\"><br/>
                    &nbsp;
                </p>
                <ul>
                    <li>
                        <p class=\"western\" align=\"justify\" style=\"\">
                            <font face=\"Times New Roman, serif\">Telefones: (61) 2022-5922 / 5854 / 5815 /5849
                                /5802/ 5946 / 5844 /5383 /5836/4131
                            </font>
                        </p>
                    </li>
                </ul>
			";
        return $texto;
    }

    public function conselheirosQuePossuiLogin($limite, $offset) {
        $sql = "
            SELECT count(1),
                cpf_conselheiro,
                to_json(array_agg(no_conselheiro)) AS no_conselheiro,
                to_json(array_agg(co_mun_ibge_conselho)) AS co_mun_ibge_conselho,
                to_json(array_agg(esf_adm_conselho)) AS esf_adm_conselho,
                to_json(array_agg(uf_conselho)) AS uf_conselho,
                to_json(array_agg(ds_funcao)) AS ds_funcao,
                to_json(array_agg(email_conselheiro)) AS email_conselheiro,
                substring(cpf_conselheiro from 1 for 3) || 'CACS' AS senha
            FROM par3.cacs
            WHERE cpf_conselheiro IN (
                SELECT cpf_conselheiro FROM par3.cacs
                INTERSECT
                SELECT usucpf FROM seguranca.usuario
                )

            AND email_conselheiro != ''
            AND trim(sit_mandato) = 'REGULAR'
            AND trim(sit_conselheiro) ILIKE 'REGULAR%'
            GROUP BY cpf_conselheiro OFFSET {$offset} LIMIT {$limite};
        ";

        $dados = $this->carregar($sql);
        return (!empty($dados) ? $dados : array());
    }

    public function validaDadosConselheiro($arrCacsModificados) {
        $arrInsert = array();
        $cont = 0;
        foreach ($arrCacsModificados as $k => $linha) {
            
            $erro = false;
            $descErro = '';

            if ((is_array($linha)) && (count($linha) > 0 )) {
                $emailConselheiro = trim($linha['email_conselheiro']);
               $emailConselho = trim($linha['email_conselho']);

                if (!$this->validaEmailCacs($emailConselheiro) || ($emailConselheiro == '')) {
                    $descErro = 'E-mail conselheiro inválido';
                    $erro = true;
                }

//                if (($emailConselheiro == '')) {
//                    $descErro = 'E-mail conselheiro vazio';
//                    $erro = true;
//                }
//
//                if (!$this->validaEmailCacs($emailConselho)) {
//                    $descErro = 'E-mail conselho inválido';
//                    $erro = true;
//                }

                if (!$erro) {
                    foreach ($linha as $campo => $k) {
                        if (($campo == 'esf_adm_conselho') ||
                                ($campo == 'sit_mandato') ||
                                ($campo == 'uf_conselho') ||
                                ($campo == 'mun_conselho') ||
                                ($campo == 'no_conselheiro') ||
                                ($campo == 'sit_conselheiro') ||
                                ($campo == 'tp_membro') ||
                                ($campo == 'ds_funcao')
                        ) {
                            $arrInsert[$cont][$campo] = ereg_replace("[^a-zA-Z0-9_, â, Â, à, À, á, Á,., ã, Ã, ê, Ê, è, È, é, É, î, Î, ì, Ì, í, Í, õ, Õ, ô, Ô, ò, Ò, ó, Ó, û, Û, ú, Ú, ù, Ù, ç, Ç,(,),]", "", $k);
                        } else if (($campo == 'co_mun_ibge_conselho') ||
                                ($campo == 'cpf_conselheiro')
                        ) {
                            $arrInsert[$cont][$campo] = ereg_replace("[^0-9]", "", $k);
                        } else if ($campo == 'email_conselho') {
                            $arrInsert[$cont][$campo] = $emailConselho;
                        } else if ($campo == 'email_conselheiro') {
                            $arrInsert[$cont][$campo] = $emailConselheiro;
                        } else if ($campo == 'dt_ult_carga') {
                            $arrInsert[$cont][$campo] = $linha['dt_ult_carga'];
                        } else if ($campo == 'senha') {
                            $arrInsert[$cont][$campo] = $linha['senha'];
                        }
                    }
                } else {
                    $this->insereLogErro($linha['cpf_conselheiro'], $descErro);
                }
            }
            $cont++;
        }
        return $arrInsert;
    }

    public function validaEmailCacs($email) {
        $conta = "^[a-zA-Z0-9\._-]+@";
        $domino = "[a-zA-Z0-9\._-]+.";
        $extensao = "([a-zA-Z]{2,4})$";
        $pattern = $conta . $domino . $extensao;
        if (preg_match($pattern, $email))
            return true;
        else
            return false;
    }

    public function insereLogErro($usucpf, $descErro) {

        $sql = "INSERT INTO par.log_carga_cacs (cpf_conselheiro, lccdescerro) VALUES ('{$usucpf}', '$descErro')";
        $this->executar($sql);

        return $this->commit();
    }

    public function retonarTamanhoTabelaComEspelhoCacs() {

        $sql = "SELECT 
                    COUNT(*)
                FROM integracao.cacs c
                LEFT JOIN integracao.espelho_cacs esc ON c.cpf_conselheiro = esc.cpf_conselheiro
                                                         AND c.co_mun_ibge_conselho = esc.co_mun_ibge_conselho
                                                         AND c.esf_adm_conselho = esc.esf_adm_conselho
               WHERE esc.cpf_conselheiro ISNULL; /*OR
                  (
                  c.co_mun_ibge_conselho <> esc.co_mun_ibge_conselho
                    OR c.esf_adm_conselho <> esc.esf_adm_conselho
                  OR c.sit_mandato <> esc.sit_mandato
                  OR c.uf_conselho <> esc.uf_conselho
                  OR c.mun_conselho <> esc.mun_conselho
                  --OR c.email_conselho <> esc.email_conselho
                  OR c.cpf_conselheiro <> esc.cpf_conselheiro
                  OR c.no_conselheiro <> esc.no_conselheiro
                  OR c.email_conselheiro  <> esc.email_conselheiro
                  OR c.sit_conselheiro <> esc.sit_conselheiro
                  --OR c.ds_segmento <> esc.ds_segmento
                  --OR c.tp_membro <> esc.tp_membro
                  OR c.ds_funcao <> esc.ds_funcao
                  )*/";

        $result = adapterConnection::coorporativo()->pegaUm($sql);

        return $result;
    }
    
    public function comparaTabelaComEspelhoCacs($offset) {

        $sql = "SELECT DISTINCT 
                        c.co_mun_ibge_conselho,
                        c.esf_adm_conselho,
                        c.sit_mandato,
                        c.uf_conselho,
                        c.mun_conselho,
                        c.email_conselho,
                        c.cpf_conselheiro,
                        c.no_conselheiro,
                        c.email_conselheiro,
                        c.sit_conselheiro,
                        c.ds_segmento,
                        c.tp_membro,
                        c.ds_funcao,
                        c.dt_ult_carga,
                        substring(c.cpf_conselheiro from 1 for 3) || 'CACS' AS senha
                FROM integracao.cacs c
                LEFT JOIN integracao.espelho_cacs esc ON c.cpf_conselheiro = esc.cpf_conselheiro
                                                         AND c.co_mun_ibge_conselho = esc.co_mun_ibge_conselho
                                                         AND c.esf_adm_conselho = esc.esf_adm_conselho
               WHERE esc.cpf_conselheiro ISNULL /*OR
                  (
                  c.co_mun_ibge_conselho <> esc.co_mun_ibge_conselho
                    OR c.esf_adm_conselho <> esc.esf_adm_conselho
                  OR c.sit_mandato <> esc.sit_mandato
                  OR c.uf_conselho <> esc.uf_conselho
                  OR c.mun_conselho <> esc.mun_conselho
                  --OR c.email_conselho <> esc.email_conselho
                  OR c.cpf_conselheiro <> esc.cpf_conselheiro
                  OR c.no_conselheiro <> esc.no_conselheiro
                  OR c.email_conselheiro  <> esc.email_conselheiro
                  OR c.sit_conselheiro <> esc.sit_conselheiro
                  --OR c.ds_segmento <> esc.ds_segmento
                  --OR c.tp_membro <> esc.tp_membro
                  OR c.ds_funcao <> esc.ds_funcao
                  )*/
                  order by 
                    cpf_conselheiro
                  limit 
                    10000 
                  offset 
                    {$offset} 
                ";
        $result = adapterConnection::coorporativo()->carregar($sql);

        return is_array($result) ? $result : array();
    }

    public function verificarExistenciaConcelheiroCacs($cpfConselheiro) {

        $sql = "SELECT 
                    cpf_conselheiro
                FROM par3.cacs
                WHERE 
                    cpf_conselheiro = '{$cpfConselheiro}'";

        $result = $this->pegaUm($sql);
        return $result;
    }

    public function preencherCacs($offset, $rotinaCacs = true, $cpf = null) {

        if($rotinaCacs) {
            $arrEspelhoCacs = $this->ComparaTabelaComEspelhoCacs($offset);
        } elseif (!$rotinaCacs && !empty($cpf)) {
            $arrEspelhoCacs = $this->buscaDadosConselheiro($cpf);
        } else {
            $arrEspelhoCacs = false;
        }

        if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

            $arrEspelhoCacs = $this->validaDadosConselheiro($arrEspelhoCacs);

            if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

                foreach ($arrEspelhoCacs as $cacs) {

                    try {
                        if (!($this->verificarExistenciaConcelheiroCacs($cacs['cpf_conselheiro']))) {

                            $sql = "
                            INSERT INTO par3.cacs(
                                co_mun_ibge_conselho,
                                esf_adm_conselho,
                                sit_mandato,
                                uf_conselho,
                                mun_conselho,
                                email_conselho,
                                cpf_conselheiro,
                                no_conselheiro,
                                email_conselheiro,
                                sit_conselheiro,
                                ds_segmento,
                                dt_ult_carga,
                                tp_membro,
                                ds_funcao
                            )VALUES(
                                '{$cacs['co_mun_ibge_conselho']}',
                                '{$cacs['esf_adm_conselho']}',
                                '{$cacs['sit_mandato']}',
                                '{$cacs['uf_conselho']}',
                                '{$cacs['mun_conselho']}',
                                '{$cacs['email_conselho']}',
                                '{$cacs['cpf_conselheiro']}',
                                '{$cacs['no_conselheiro']}',
                                '{$cacs['email_conselheiro']}',
                                '{$cacs['sit_conselheiro']}',
                                '{$cacs['ds_segmento']}',
                                '{$cacs['dt_ult_carga']}',
                                '{$cacs['tp_membro']}',
                                '{$cacs['ds_funcao']}'
                            )";
                        } else {
                            $sql = "
                            UPDATE
                                par3.cacs
                            SET
                                co_mun_ibge_conselho = '{$cacs['co_mun_ibge_conselho']}',
                                esf_adm_conselho = '{$cacs['esf_adm_conselho']}',
                                sit_mandato = '{$cacs['sit_mandato']}',
                                uf_conselho = '{$cacs['uf_conselho']}',
                                mun_conselho = '{$cacs['mun_conselho']}',
                                email_conselho = '{$cacs['email_conselho']}',
                                cpf_conselheiro = '{$cacs['cpf_conselheiro']}',
                                no_conselheiro = '{$cacs['no_conselheiro']}',
                                email_conselheiro = '{$cacs['email_conselheiro']}',
                                sit_conselheiro = '{$cacs['sit_conselheiro']}',
                                ds_segmento = '{$cacs['ds_segmento']}',
                                dt_ult_carga = '{$cacs['dt_ult_carga']}',
                                tp_membro = '{$cacs['tp_membro']}',
                                ds_funcao = '{$cacs['ds_funcao']}'
                            WHERE
                                cpf_conselheiro = '{$cacs['cpf_conselheiro']}'";
                        }

                        $this->executar($sql);
                        $this->commit();
                    } catch (Exception $exc) {
                        echo $exc->getTraceAsString();
                        return false;
                    }
                }
            } else {
                return false;
            }
        } else {
            return false;
        }
        return true;
    }

    public function verificarUsuarioCacs($usuCpf) {
        $sql = "SELECT 
                    usucpf 
                FROM seguranca.usuario 
                WHERE usucpf = '{$usuCpf}'";

        $result = $this->pegaUm($sql);
        return $result;
    }

    public function salvarUsuarioCacsNovo($offset, $rotinaCacs = true, $cpf = null) {

        if($rotinaCacs) {
            $arrEspelhoCacs = $this->ComparaTabelaComEspelhoCacs($offset);
        } elseif (!$rotinaCacs && !empty($cpf)) {
            $arrEspelhoCacs = $this->buscaDadosConselheiro($cpf);
        } else {
            $arrEspelhoCacs = false;
        }

        if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

            $arrEspelhoCacs = $this->validaDadosConselheiro($arrEspelhoCacs);

            if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

                $ctrlUsuario = new Seguranca_Controller_Usuario();
                $usuario = new Seguranca_Model_Seguranca_Usuario();
                $usuarioSistema1 = new Seguranca_Model_Seguranca_UsuarioSistema();
                $usuarioSistema2 = new Seguranca_Model_Seguranca_UsuarioSistema();
                $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
                $perfilUsuario = new Seguranca_Model_Seguranca_PerfilUsuario();

                foreach ($arrEspelhoCacs as $espelhoCacs => $cacs) {

                    $cacs = $this->popularUsuarioConselheiro($cacs);

                    // busca perfil par2
                    $pflcodParAntigo = $this->getPerfilCacs(array($cacs['ds_funcao']), true);
                    // busca perfil par3
                    $pflcod = $this->getPerfilCacs(array($cacs['ds_funcao']));

                    if (!($this->verificarUsuarioCacs($cacs['usucpf']))) {

                        $usuario->salvarUsuario($cacs, 'A'); //retirar bloqueio

                        $usuarioSistema1->salvarUsuarioSistema($cacs['usucpf'], PAR3_SIS_ID); //retirar bloqueio
                        $usuarioSistema2->salvarUsuarioSistema($cacs['usucpf'], SISID_PAR_02);

                        /** SALVAR USUARIO RESPONSABILIDADE */
                        $usuarioResponsabilidade->salvarUsuarioResponsabilidade($cacs, $pflcod); //retirar bloqueio
                        $usuarioResponsabilidade->salvarUsuarioResponsabilidade($cacs, $pflcodParAntigo, 'par.usuarioresponsabilidade');

                        /** SALVAR USUARIO SISTEMA */
                        $perfilUsuario->salvarPerfil($cacs['usucpf'], $pflcod); //retirar bloqueio
                        $perfilUsuario->salvarPerfil($cacs['usucpf'], $pflcodParAntigo);

                        /** SALVAR PEFIL USUARIO */
                        $usuario->commit();
                        $usuarioSistema1->commit();
                        $usuarioSistema2->commit();
                        $usuarioResponsabilidade->commit();
                        $perfilUsuario->commit();
                        if (IS_PRODUCAO) {
                            $this->enviarEmailCacs($cacs);
                        }
                    } else {
                        $ctrlUsuario->atualizaDadosUsuarioCacs($cacs, $pflcodParAntigo, SISID_PAR_02, $rotinaCacs);
                        $ctrlUsuario->atualizaDadosUsuarioCacs($cacs, $pflcod, PAR3_SIS_ID, $rotinaCacs);
                    }
                }
            }
        }
        return true;
    }

    public function atualizaEspelhoCacs() {

        $sqlDelete = "DELETE FROM integracao.espelho_cacs";

        $result1 = adapterConnection::coorporativo()->executar($sqlDelete);

        $sql = "INSERT INTO integracao.espelho_cacs (co_mun_ibge_conselho, esf_adm_conselho, sit_mandato,
                                     uf_conselho, mun_conselho, email_conselho,
                                     cpf_conselheiro, no_conselheiro, email_conselheiro,
                                     sit_conselheiro, ds_segmento, tp_membro,
                                     ds_funcao, dt_ult_carga)
                (SELECT co_mun_ibge_conselho, esf_adm_conselho, sit_mandato,
                                     uf_conselho, mun_conselho, email_conselho,
                                     cpf_conselheiro, no_conselheiro, email_conselheiro,
                                     sit_conselheiro, ds_segmento, tp_membro,
                                     ds_funcao, dt_ult_carga FROM integracao.cacs)";

        $result2 = adapterConnection::coorporativo()->executar($sql);

        return adapterConnection::coorporativo()->commit();
    }

    /**
     * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
     * @description esta função altera a senha do usuário membro do CACS ou realizar o cadastro do mesmo se ele nao possuir uruário no SIMEC
     * @return boolean
     */
    public function alterarSenhaUsuarioCacs($cpf){

        $perfilUsuario = new Seguranca_Model_Seguranca_PerfilUsuario();

        //Verifica se o usuário possui perfil de conselheiro CACS no SIMEC
        if($perfilUsuario->verficaPerfilCacs($cpf)){
            if($this->salvarUsuarioCacsNovo(0, false, $cpf)){
                return true;
            }
        } else {
            //Atualiza dados do conselheiro na tabela par3.cacs
            if($this->preencherCacs(0, false, $cpf)){
                //Cadastra ou atualiza os dados do conselheiro no SIMEC
                if($this->salvarUsuarioCacsNovo(0, false, $cpf)){
                   return true;
                }
            }
        }
        return false;
    }

    /**
     * @author Sérgio Henrique <sergio.hsilva@mec.gov.br>
     * @description esta função busca os dados de um membro do CACS atraves do seu cpf
     * @return array
     */
    public function buscaDadosConselheiro($cpf){

        $sql = "SELECT DISTINCT 
                        c.co_mun_ibge_conselho,
                        c.esf_adm_conselho,
                        c.sit_mandato,
                        c.uf_conselho,
                        c.mun_conselho,
                        c.email_conselho,
                        c.cpf_conselheiro,
                        c.no_conselheiro,
                        c.email_conselheiro,
                        c.sit_conselheiro,
                        c.ds_segmento,
                        c.tp_membro,
                        c.ds_funcao,
                        c.dt_ult_carga,
                        substring(c.cpf_conselheiro from 1 for 3) || 'CACS' AS senha
                FROM integracao.cacs c WHERE c.cpf_conselheiro = '{$cpf}'
                ";
        $result = adapterConnection::coorporativo()->carregar($sql);

        return is_array($result) ? $result : array();
    }

    public function inativaConselheiroCacs($offset){
        $sql = "SELECT ec.cpf_conselheiro as usucpf 
                FROM integracao.espelho_cacs ec
                LEFT JOIN integracao.cacs c ON ec.cpf_conselheiro = c.cpf_conselheiro
                WHERE c.cpf_conselheiro ISNULL AND ec.ds_funcao = 'PRESIDENTE' OFSSET {$offset} LIMIT 500 ";

        $dados = adapterConnection::coorporativo()->carregar($sql);
        (!empty($dados) ? $dados : array());

        if(count($dados) > 0){
            foreach ($dados as $usuario){

            $strPerfilCacs = Par3_Model_UsuarioResponsabilidade::PRESIDENTE_CACS . "," . Par3_Model_UsuarioResponsabilidade::CONSELHEIRO_CACS . "," .
                PAR_PERFIL_CONSELHEIRO . "," . PAR_PERFIL_PRESIDENTE;

            $resposabilidadePar = "UPDATE par.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usuario['usucpf']}'";
            $resposabilidadePar3 = "UPDATE par3.usuarioresponsabilidade SET rpustatus = 'I' WHERE usucpf = '{$usuario['usucpf']}'";
            $sqlDelete = "DELETE FROM seguranca.perfilusuario WHERE usucpf = '{$usuario['usucpf']}' AND pflcod IN ({$strPerfilCacs});";
            $sqlCacs = "DELETE FROM par3.cacs WHERE cpf_conselheiro = '{$usuario['usucpf']}'";

            $this->executar($resposabilidadePar);
            $this->executar($resposabilidadePar3);
            $this->executar($sqlDelete);
            $this->executar($sqlCacs);
            $this->commit();
            }
        }
    }

    public function retonarQtdInativar(){
        $sql = "SELECT ec.cpf_conselheiro as usucpf 
                FROM integracao.espelho_cacs ec
                LEFT JOIN integracao.cacs c ON ec.cpf_conselheiro = c.cpf_conselheiro
                WHERE c.cpf_conselheiro ISNULL AND ec.ds_funcao = 'PRESIDENTE'";

        $result = adapterConnection::coorporativo()->pegaUm($sql);

        return $result;
    }

    public function insereUsuarioCacs($conselheiro) {
        $cacs = $this->validaDadosCACS($conselheiro);

        if (is_array($cacs) && !empty($cacs)) {

            try {
                if (!($this->verificarExistenciaConcelheiroCacs($cacs['cpf_conselheiro']))) {

                    $sql = "
                            INSERT INTO par3.cacs(
                                co_mun_ibge_conselho,
                                esf_adm_conselho,
                                sit_mandato,
                                uf_conselho,
                                mun_conselho,
                                email_conselho,
                                cpf_conselheiro,
                                no_conselheiro,
                                email_conselheiro,
                                sit_conselheiro,
                                ds_segmento,
                                dt_ult_carga,
                                tp_membro,
                                ds_funcao
                            )VALUES(
                                '{$cacs['co_mun_ibge_conselho']}',
                                '{$cacs['esf_adm_conselho']}',
                                '{$cacs['sit_mandato']}',
                                '{$cacs['uf_conselho']}',
                                '{$cacs['mun_conselho']}',
                                '{$cacs['email_conselho']}',
                                '{$cacs['cpf_conselheiro']}',
                                '{$cacs['no_conselheiro']}',
                                '{$cacs['email_conselheiro']}',
                                '{$cacs['sit_conselheiro']}',
                                '{$cacs['ds_segmento']}',
                                'now()',
                                '{$cacs['tp_membro']}',
                                '{$cacs['ds_funcao']}'
                            )";
                } else {
                    $sql = "
                            UPDATE
                                par3.cacs
                            SET
                                co_mun_ibge_conselho = '{$cacs['co_mun_ibge_conselho']}',
                                esf_adm_conselho = '{$cacs['esf_adm_conselho']}',
                                sit_mandato = '{$cacs['sit_mandato']}',
                                uf_conselho = '{$cacs['uf_conselho']}',
                                mun_conselho = '{$cacs['mun_conselho']}',
                                email_conselho = '{$cacs['email_conselho']}',
                                cpf_conselheiro = '{$cacs['cpf_conselheiro']}',
                                no_conselheiro = '{$cacs['no_conselheiro']}',
                                email_conselheiro = '{$cacs['email_conselheiro']}',
                                sit_conselheiro = '{$cacs['sit_conselheiro']}',
                                ds_segmento = '{$cacs['ds_segmento']}',
                                dt_ult_carga = 'now()',
                                tp_membro = '{$cacs['tp_membro']}',
                                ds_funcao = '{$cacs['ds_funcao']}'
                            WHERE
                                cpf_conselheiro = '{$cacs['cpf_conselheiro']}'";
                }

                $this->executar($sql);
                $this->commit();
                $this->salvarUsuarioCacsV2($cacs);


            } catch (Exception $exc) {
                echo $exc->getTraceAsString();
                return false;
            }
        }
    }

    public function validaDadosCACS($conselheiro) {
        $arrInsert = array();

        $erro = false;
        $descErro = '';


        if ((is_array($conselheiro)))  {
            $emailConselheiro = trim($conselheiro['email_conselheiro']);
            $emailConselho = trim($conselheiro['email_conselho']);

            if (!$this->validaEmailCacs($emailConselheiro) || ($emailConselheiro == '')) {
                $descErro = 'E-mail conselheiro inválido';
                $erro = true;
            }


            if (!$erro) {
                foreach ($conselheiro as $campo => $k) {

                    if (($campo == 'esf_adm_conselho') ||
                        ($campo == 'sit_mandato') ||
                        ($campo == 'uf_conselho') ||
                        ($campo == 'mun_conselho') ||
                        ($campo == 'no_conselheiro') ||
                        ($campo == 'sit_conselheiro') ||
                        ($campo == 'tp_membro') ||
                        ($campo == 'ds_funcao')
                    ) {
                        $arrInsert[$campo] = ereg_replace("[^a-zA-Z0-9_, â, Â, à, À, á, Á,., ã, Ã, ê, Ê, è, È, é, É, î, Î, ì, Ì, í, Í, õ, Õ, ô, Ô, ò, Ò, ó, Ó, û, Û, ú, Ú, ù, Ù, ç, Ç,(,),]", "", $k);
                    } else if (($campo == 'co_mun_ibge_conselho') ||
                        ($campo == 'cpf_conselheiro')
                    ) {
                        $arrInsert[$campo] = ereg_replace("[^0-9]", "", $k);
                    } else if ($campo == 'email_conselho') {
                        $arrInsert[$campo] = $emailConselho;
                    } else if ($campo == 'email_conselheiro') {
                        $arrInsert[$campo] = $emailConselheiro;
                    } else if ($campo == 'dt_ult_carga') {
                        $arrInsert[$campo] = $conselheiro['dt_ult_carga'];
                    } else if ($campo == 'senha') {
                        $arrInsert[$campo] = $conselheiro['senha'];
                    }
                }
            } else {
                $this->insereLogErro($conselheiro['cpf_conselheiro'], $descErro);
            }
        }

        return $arrInsert;
    }

    public function salvarUsuarioCacsV2($arrEspelhoCacs) {

        if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

            $arrEspelhoCacs = $this->validaDadosCACS($arrEspelhoCacs);

            if (is_array($arrEspelhoCacs) && !empty($arrEspelhoCacs)) {

                $ctrlUsuario = new Seguranca_Controller_Usuario();
                $usuario = new Seguranca_Model_Seguranca_Usuario();
                $usuarioSistema1 = new Seguranca_Model_Seguranca_UsuarioSistema();
                $usuarioSistema2 = new Seguranca_Model_Seguranca_UsuarioSistema();
                $usuarioResponsabilidade = new Par3_Model_UsuarioResponsabilidade();
                $perfilUsuario = new Seguranca_Model_Seguranca_PerfilUsuario();


                    $cacs = $this->popularUsuarioConselheiro($arrEspelhoCacs);

                    // busca perfil par2
                    $pflcodParAntigo = $this->getPerfilCacs(array($cacs['ds_funcao']), true);
                    // busca perfil par3
                    $pflcod = $this->getPerfilCacs(array($cacs['ds_funcao']));

                    if (!($this->verificarUsuarioCacs($cacs['usucpf']))) {

                        $usuario->salvarUsuario($cacs, 'A'); //retirar bloqueio

                        $usuarioSistema1->salvarUsuarioSistema($cacs['usucpf'], PAR3_SIS_ID); //retirar bloqueio
                        $usuarioSistema2->salvarUsuarioSistema($cacs['usucpf'], SISID_PAR_02);

                        /** SALVAR USUARIO RESPONSABILIDADE */
                        $usuarioResponsabilidade->salvarUsuarioResponsabilidade($cacs, $pflcod); //retirar bloqueio
                        $usuarioResponsabilidade->salvarUsuarioResponsabilidade($cacs, $pflcodParAntigo, 'par.usuarioresponsabilidade');

                        /** SALVAR USUARIO SISTEMA */
                        $perfilUsuario->salvarPerfil($cacs['usucpf'], $pflcod); //retirar bloqueio
                        $perfilUsuario->salvarPerfil($cacs['usucpf'], $pflcodParAntigo);

                        /** SALVAR PEFIL USUARIO */
                        $usuario->commit();
                        $usuarioSistema1->commit();
                        $usuarioSistema2->commit();
                        $usuarioResponsabilidade->commit();
                        $perfilUsuario->commit();
                        if (IS_PRODUCAO) {
                            $this->enviarEmailCacs($cacs);
                        }
                    } else {
                        $ctrlUsuario->atualizaDadosUsuarioCacs($cacs, $pflcodParAntigo, SISID_PAR_02, $rotinaCacs);
                        $ctrlUsuario->atualizaDadosUsuarioCacs($cacs, $pflcod, PAR3_SIS_ID, $rotinaCacs);
                    }

            }
        }
    }
}
