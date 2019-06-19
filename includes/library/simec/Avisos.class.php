<?php

class Avisos
{

    public function __construct()
    {}

    /*
     * Parametros do array:
     * array(
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * ..................
     * )
     *
     */
    function getAvisoOmissaoPCPAR3($arrParams = array(), $adm = false)
    {
        
        if (count($arrParams) > 0) {
            $prefeitoSecretario = $arrParams['prefeito_secretario'];
            
            $responsavelEntidadePar = ($arrParams['dadosUnidade']['dadosInstrumento']['itrid'] == 1) ? 'Secretário(a) Estadual de Educação' : 'Prefeito(a)'; //@todo
            
            $inuid = $arrParams['inuid'];
            
            if ($prefeitoSecretario) {
                $strRetorno .= '<p style="margin: 10px 0px;"> Senhor(a) '.$responsavelEntidadePar.',</p>';
                $strRetorno .= '<p style="margin: 10px 0px;"> 1. Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC o envio da prestação de contas relativa ao(s) Termo(s) de Compromisso abaixo identificado(s)</b>:</p>';
            
                
            
                if (count($arrParams) == 1) {
                    $size = 'large';
                }
            
                if (count($arrParams) == 2) {
                    $size = 'medium';
                }
            
                if (count($arrParams) == 3) {
                    $size = 'small';
                }
            
                $strRetorno .= "<table align=\"center\" class=\"tabela\" border=\"1px\">
			    <thead>
			    	<tr >
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Termo de Compromisso</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Processo</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Convenente</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">CNPJ</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Prazo para prestar Contas</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Início  Fim Vigência</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Valor total repassado</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Programa</td>
                        <td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Saldo</td>
			    	</tr>
			    </thead>
			    <tbody>";
            
                $processos = ($arrParams['arrRegistros']) ? $arrParams['arrRegistros'] : Array();
                if (count($processos) > 0) {
                    foreach ($processos as $k => $processo) {
            
                        // $strRetorno .= $processo ['numeroprocesso'];
                        $strRetorno .= "
                        <tr>
                        <td align=\"center\"  style=\"FILTER:\">{$processo['numero_termo']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px; \">{$processo['processo']} </td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['nome_entidade']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['cnpj_entidade']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_prestar_contas']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_vigencia']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['valor_repassado'])."</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['programa']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['saldo'])."</td>
                        </tr>
                        ";
                    }
                }
            
                $strRetorno .= "
			    </tbody>
			    </table>";
            
                $strRetorno .= '<p style="margin: 10px 0px;"> 2.    Considerando os períodos dos créditos dos recursos e da execução do(s) Termo(s) de Compromisso, bem como o(s) prazo(s) para prestar contas, foram identificados outros responsáveis, conforme disposto no Rol de Responsáveis inerente a cada Transferência no SIMEC, que também serão notificados e orientados a adotar as providências necessárias.</p>';
            
                $strRetorno .= '<p style="margin: 10px 0px;">3.	A obrigação de prestar contas dá-se nos termos do Art. 70 da Constituição Federal de 1988 e demais normativos aplicáveis. Dessa forma, concedemos o prazo de 15 (quinze) dias, a contar da data da ciência deste Ofício, para o saneamento da pendência ou para a devolução dos recursos, devidamente atualizados. O não atendimento a esta diligência no prazo estabelecido implicará a inadimplência da Entidade e a adoção da devida medida de exceção, instauração de Tomada de Contas Especial ou o registro no Cadastro Informativo de Créditos Não Quitados do Setor Público Federal (Cadin), sendo que este último ocorrerá em 75 (setenta e cinco) dias a contar do fim do prazo concedido inicialmente. </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">4.	Em razão do disposto Resolução nº 12, de 6 de junho de 2018, que institui a obrigatoriedade do uso Sistema Integrado de Monitoramento Execução e Controle - SIMEC, deve-se proceder à devida inserção dos dados no referido Sistema, o qual é  acessado no endereço:  http://simec.mec.gov.br, sendo que a documentação apresentada fisicamente não é válida para comprovação.  </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">5.	Caso não disponha da documentação necessária, orientamo-lo a manter contato com os demais responsáveis, visando ao saneamento da pendência ou a devolução dos recursos, ou, na impossibilidade de fazê-lo, adotar as medidas legais visando o resguardo do patrimônio público. Optando Vossa Excelência pela adoção das medidas legais, é necessário que apresente justificativa ao FNDE, obrigatoriamente, acompanhada de cópia autenticada de representação protocolizada junto ao respectivo órgão do Ministério Público, para adoção das providências cíveis e criminais da sua alçada, de acordo com o Manual de Assistência Financeira do FNDE (Resolução CD/FNDE nº 53, de 29/10/2009). </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">6.	Caso opte pela devolução total dos recursos, deve-se atualizar o valor do débito na data do pagamento, com base no valor e data das Ordens Bancárias indicados no Sítio do FNDE - https://www.fnde.gov.br/sigefweb/index.php/liberacoes  ou no SIMEC, utilizando-se o Sistema Débito, no Sítio do Tribunal de Contas da União - http://portal.tcu.gov.br/sistema-atualizacao-de-debito/ , e recolher via Guia de Recolhimento da União, conforme instruções constantes no endereço http://www.fnde.gov.br/acoes/prestacao-de-contas/area-para-gestores/gru-devolucao-de-recursos-financeiros . O índice tem correção mensal, razão pela qual o valor a ser restituído deverá ser atualizado na data da efetiva devolução </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">7.	Alertamos que Vossa Excelência é responsável pela devolução do(s) saldo(s) existente(s) na(s)  conta(s)  específica(s)  (conta corrente e conta aplicação), conforme apontado nos quadros constantes no parágrafo 1 desta Diligência. Essa devolução também deverá ser atualizada pelo Sistema Débito e recolhida por GRU. Ressaltamos que o saldo de aplicação não necessita de atualização. Caso verifique que exista saldo, orientamos que efetue o recolhimento e declare na respectiva prestação de contas, enviando-a via SIMEC ao FNDE, ainda que não possua os demais registros da execução, o que sanará a responsabilidade de Vossa Excelência perante o saldo verificado quando da análise por esta Autarquia. </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">8.	Em tempo, salientamos que nos casos de entidades privadas sem fins lucrativos, de acordo com o incidente de uniformização de jurisprudência do TCU exposta na Súmula n° 286 - TCU - Plenário, as medidas restritivas e de exceção levarão em conta a responsabilização solidária da entidade pelo dano causado ao erário. </p>';
                
            
                $strRetorno .= "
				    <div align='center' ><input type=\"checkbox\"
	                                                                                         onclick=\"liberaBotao()\"
	                                                                                         name=\"checkbox_ciencia_omissao\" id=\"checkbox_ciencia_omissao\"
	                                                                                         onclick=\"\"
	                                                                                         >&nbsp;
	                        <label style=\"font-size: 12px\">Declaro ciência da notificação por omissão de prestar contas do(s) Termo(s) listado(s) acima.</label> </div>";
            
                $botao = " <button type=\"button\"
                
                        onclick=\"declaraCiencia();\"
                        class=\"btn btn-default center\">
                    <i class=\"\"></i> Confirmar
                </button>";
            } else {
            
                $strRetorno .= "
                     <p style=\"margin: 10px 0px;\"> 
Existe Notificação Eletrônica encaminhada ao Dirigente máximo da Entidade ".$arrParams['dadosUnidade']['dadosUnidade']['entnome']." que carece de leitura e ciência.
Apenas o destinatário pode acessar o documento, por meio do seu login e senha.
Tão logo o Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC verifique a ciência à citada Notificação Eletrônica, os acessos dos demais usuários da Entidade serão reestabelecidos .
                    </p>
                    ";
                $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                $strRetorno .= "</div>";
            }
            
            
            
            
            
             
            
            echo "
            <style>
             
            </style>
            
            <div class=\"ibox float-e-margins animated modal\" id=\"modal_iniciativaDesc\" tabindex=\"-1\" role=\"dialog\" aria-hidden=\"true\">
            <div class=\"modal-dialog modal-lg\" style=\"width: 95%;\">
            <div class=\"modal-content\">
            
            <div class=\"ibox-content\" id=\"conteudo_modal_contrato\">
            <!-- Conteúdo -->
            <div align='' >
            {$strRetorno}
            
            </div>";
            if($prefeitoSecretario) {
                echo "<div class=\"ibox-footer center\" id=\"footerModalContrato\">
                        {$botao}
                    </div>";
            }
            echo "
            </div>
            </div>
            </div>
            
            ";
        }
        

    }
    
    /*
     * Parametros do array:
     * array(
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * ..................
     * )
     *
     *$ropid = 1 Acesso do gestor ou equipe, 2 = acesso Presidente CACS ou equipe
     */
    function getAvisoOmissaoPCPAR3CACS($arrParams = array(), $ropid)
    {
        
        if (count($arrParams) > 0) {
            
            $prefeitoSecretario = $arrParams['responsavel'];
    
            $responsavelEntidadePar = ($arrParams['dadosUnidade']['dadosInstrumento']['itrid'] == 1) ? 'Secretário(a) Estadual de Educação' : 'Prefeito(a)'; //@todo
    
            $inuid = $arrParams['inuid'];
    
            if ($prefeitoSecretario) {
                $strRetorno .= '<p style="margin: 10px 0px;"> Senhor(a) '.$responsavelEntidadePar.',</p>';
                $strRetorno .= '<p style="margin: 10px 0px;"> Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC registro de envio do parecer da(s) prestação(ões) de contas relativa ao(s) Termo(s) de Compromisso abaixo identificado(s):</p>';
    
                if (count($arrParams) == 1) {
                    $size = 'large';
                }
    
                if (count($arrParams) == 2) {
                    $size = 'medium';
                }
    
                if (count($arrParams) == 3) {
                    $size = 'small';
                }
    
                $strRetorno .= "<table align=\"center\" class=\"tabela\" border=\"1px\">
			    <thead>
			    	<tr >
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Termo de Compromisso</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Processo</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Convenente</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">CNPJ</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Prazo para envio da Análise do CACS</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Início  Fim Vigência</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Valor total repassado</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Programa</td>
                        <td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Saldo</td>
			    	</tr>
			    </thead>
			    <tbody>";
    
                $processos = ($arrParams['arrRegistros']) ? $arrParams['arrRegistros'] : Array();
                if (count($processos) > 0) {
                    foreach ($processos as $k => $processo) {
    
                        // $strRetorno .= $processo ['numeroprocesso'];
                        $strRetorno .= "
                        <tr>
                        <td align=\"center\"  style=\"FILTER:\">{$processo['numero_termo']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px; \">{$processo['processo']} </td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['nome_entidade']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['cnpj_entidade']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_prestar_contas']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_vigencia']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['valor_repassado'])."</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['programa']}</td>
                        <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['saldo'])."</td>
                        </tr>
                        ";
                    }
                }
    
                $strRetorno .= "
			    </tbody>
			    </table>";
    
                $strRetorno .= '<p style="margin: 10px 0px;"> A obrigação do Conselho de Controle e Acompanhamento Social - CACS de acompanhar e manifestar-se em relação as prestações de contas de termos de compromisso no âmbito do Plano de Ações Articulas  PAR está prevista nos termos dos Art. 24 da Lei nº 11.494/2007 c/c art. 10 da Lei nº 12.695/12 e demais normativos aplicáveis.</p>';
    
                $strRetorno .= '<p style="margin: 10px 0px;">Com efeito, esgotado o referido prazo sem envio do parecer, concedemos àquele CACS o prazo de 15 (quinze) dias, a contar da data da ciência deste Ofício, para o saneamento da pendência.</p>';
                $strRetorno .= '<p style="margin: 10px 0px;">Dessa forma, sugerimos a Vossa Excelência a adoção de medidas cabíveis para fins de saneamento da referida pendência, sob pena de suspensão de repasses de recursos do programa ou projeto, conforme estabelece o § 3º do art. 1º da Resolução CD/FNDE nº 22/2014.</p>';
                $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                $strRetorno .= "Equipe PAR MEC/FNDE</p>";
    
                $strRetorno .= "
				    <div align='center' ><input type=\"checkbox\"
	                                                                                         onclick=\"liberaBotao()\"
	                                                                                         name=\"checkbox_ciencia_omissao\" id=\"checkbox_ciencia_omissao\"
	                                                                                         onclick=\"\"
	                                                                                         >&nbsp;
	                        <label style=\"font-size: 12px\">Declaro ciência da notificação por omissão de prestar contas do(s) Termo(s) listado(s) acima.</label> </div>";
    
                $botao = " <button type=\"button\"
    
                        onclick=\"declaraCiencia({$ropid});\"
                        class=\"btn btn-default center\">
                    <i class=\"\"></i> Confirmar
                </button>";
            } else {
                
                $strRetorno .= "
                    <p style=\"margin: 10px 0px;\">
Existe Notificação Eletrônica encaminhada ao Dirigente máximo da Entidade ".$arrParams['dadosUnidade']['dadosUnidade']['entnome']." que carece de leitura e ciência.
Apenas o destinatário pode acessar o documento, por meio do seu login e senha.
Tão logo o Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC verifique a ciência à citada Notificação Eletrônica, os acessos dos demais usuários da Entidade serão reestabelecidos .
            </p>
                    ";
                $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                $strRetorno .= "</div>";
            }
    
    
    
    
    
             
    
            echo "
            <style>
             
            </style>
    
            <div class=\"ibox float-e-margins animated modal\" id=\"modal_iniciativaDesc\" tabindex=\"-1\" role=\"dialog\" aria-hidden=\"true\">
            <div class=\"modal-dialog modal-lg\" style=\"width: 95%;\">
            <div class=\"modal-content\">
    
            <div class=\"ibox-content\" id=\"conteudo_modal_contrato\">
            <!-- Conteúdo -->
            <div align='' >
            {$strRetorno}
    
            </div>";
            if($prefeitoSecretario) {
                echo "<div class=\"ibox-footer center\" id=\"footerModalContrato\">
                {$botao}
                </div>";
            }
            echo "
            </div>
            </div>
            </div>
    
            ";
        }
    }

    function getAvisoOmissaoPC($arrParams = array(), $adm = false)
    {
        
        if (count($arrParams) > 0) {
            
            $prefeitoSecretario = $arrParams['prefeito_secretario'];
            
            $strRetorno = "<div id=\"dialog_omissao_ente\" style=\"display: none;\">";
            
            $responsavelEntidadePar = ($arrParams['dadosUnidade']['dadosInstrumento']['itrid'] == 1) ? 'Secretário(a) Estadual de Educação' : 'Prefeito(a)'; //@todo
            
            if ($prefeitoSecretario) {
                $strRetorno .= '<p style="margin: 10px 0px;"> Senhor(a) '.$responsavelEntidadePar.',</p>';
                $strRetorno .= '<p style="margin: 10px 0px;"> 1. Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC o envio da prestação de contas relativa ao(s) Termo(s) de Compromisso abaixo identificado(s)</b>:</p>';
                
                $strRetorno .= '<div style="clear: both"></div>';
            
                if (count($arrParams) == 1) {
                    $size = 'large';
                }
            
                if (count($arrParams) == 2) {
                    $size = 'medium';
                }
            
                if (count($arrParams) == 3) {
                    $size = 'small';
                }
            
                $strRetorno .= "<table align=\"center\" class=\"tabela\">
			    <thead>
			    	<tr>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Termo de Compromisso</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Processo</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Convenente</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">CNPJ</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Prazo para prestar Contas</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Início  Fim Vigência</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Valor total repassado</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Programa</td>
			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Saldo</td>
			    	</tr>
			    </thead>
			    <tbody>";
            
                $processos = ($arrParams['arrRegistros']) ? $arrParams['arrRegistros'] : Array();
                if (count($processos) > 0) {
                    foreach ($processos as $k => $processo) {
                        
                        // $strRetorno .= $processo ['numeroprocesso'];
                        $strRetorno .= "
                			    	<tr>
                			    	<td align=\"center\"  style=\"FILTER:;padding:5px;\">{$processo['numero_termo']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['processo']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['nome_entidade']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['cnpj_entidade']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_prestar_contas']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_vigencia']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['valor_repassado'])."</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['programa']}</td>
                			    	<td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['saldo'])."</td>
                			    	</tr>
                			    	";
                    }
                }
            
                $strRetorno .= "
			    </tbody>
			    </table>";
                
                $strRetorno .= '<p style="margin: 10px 0px;"> 2.    Considerando os períodos dos créditos dos recursos e da execução do(s) Termo(s) de Compromisso, bem como o(s) prazo(s) para prestar contas, foram identificados outros responsáveis, conforme disposto no Rol de Responsáveis inerente a cada Transferência no SIMEC, que também serão notificados e orientados a adotar as providências necessárias.</p>';
                
                $strRetorno .= '<p style="margin: 10px 0px;">3.	A obrigação de prestar contas dá-se nos termos do Art. 70 da Constituição Federal de 1988 e demais normativos aplicáveis. Dessa forma, concedemos o prazo de 15 (quinze) dias, a contar da data da ciência deste Ofício, para o saneamento da pendência ou para a devolução dos recursos, devidamente atualizados. O não atendimento a esta diligência no prazo estabelecido implicará a inadimplência da Entidade e a adoção da devida medida de exceção, instauração de Tomada de Contas Especial ou o registro no Cadastro Informativo de Créditos Não Quitados do Setor Público Federal (Cadin), sendo que este último ocorrerá em 75 (setenta e cinco) dias a contar do fim do prazo concedido inicialmente. </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">4.	Em razão do disposto Resolução nº 12, de 6 de junho de 2018, que institui a obrigatoriedade do uso Sistema Integrado de Monitoramento Execução e Controle - SIMEC, deve-se proceder à devida inserção dos dados no referido Sistema, o qual é  acessado no endereço:  http://simec.mec.gov.br, sendo que a documentação apresentada fisicamente não é válida para comprovação.  </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">5.	Caso não disponha da documentação necessária, orientamo-lo a manter contato com os demais responsáveis, visando ao saneamento da pendência ou a devolução dos recursos, ou, na impossibilidade de fazê-lo, adotar as medidas legais visando o resguardo do patrimônio público. Optando Vossa Excelência pela adoção das medidas legais, é necessário que apresente justificativa ao FNDE, obrigatoriamente, acompanhada de cópia autenticada de representação protocolizada junto ao respectivo órgão do Ministério Público, para adoção das providências cíveis e criminais da sua alçada, de acordo com o Manual de Assistência Financeira do FNDE (Resolução CD/FNDE nº 53, de 29/10/2009). </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">6.	Caso opte pela devolução total dos recursos, deve-se atualizar o valor do débito na data do pagamento, com base no valor e data das Ordens Bancárias indicados no Sítio do FNDE - https://www.fnde.gov.br/sigefweb/index.php/liberacoes  ou no SIMEC, utilizando-se o Sistema Débito, no Sítio do Tribunal de Contas da União - http://portal.tcu.gov.br/sistema-atualizacao-de-debito/ , e recolher via Guia de Recolhimento da União, conforme instruções constantes no endereço http://www.fnde.gov.br/acoes/prestacao-de-contas/area-para-gestores/gru-devolucao-de-recursos-financeiros . O índice tem correção mensal, razão pela qual o valor a ser restituído deverá ser atualizado na data da efetiva devolução </p>';
                $strRetorno .= '<p style="margin: 10px 0px;">7.	Alertamos que Vossa Excelência é responsável pela devolução do(s) saldo(s) existente(s) na(s)  conta(s)  específica(s)  (conta corrente e conta aplicação), conforme apontado nos quadros constantes no parágrafo 1 desta Diligência. Essa devolução também deverá ser atualizada pelo Sistema Débito e recolhida por GRU. Ressaltamos que o saldo de aplicação não necessita de atualização. Caso verifique que exista saldo, orientamos que efetue o recolhimento e declare na respectiva prestação de contas, enviando-a via SIMEC ao FNDE, ainda que não possua os demais registros da execução, o que sanará a responsabilidade de Vossa Excelência perante o saldo verificado quando da análise por esta Autarquia. </p>';//@todo
                $strRetorno .= '<p style="margin: 10px 0px;">8.	Em tempo, salientamos que nos casos de entidades privadas sem fins lucrativos, de acordo com o incidente de uniformização de jurisprudência do TCU exposta na Súmula n° 286 - TCU - Plenário, as medidas restritivas e de exceção levarão em conta a responsabilização solidária da entidade pelo dano causado ao erário. </p>';
                $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                
             
                
                
                    $strRetorno .= "
				    <div align='center' ><input type=\"checkbox\"
	                                                                                         onclick=\"liberaBotao()\"
	                                                                                         name=\"checkbox_ciencia_omissao\" id=\"checkbox_ciencia_omissao\"
	                                                                                         onclick=\"\"
	                                                                                         >&nbsp;
	                        <label style=\"font-size: 12px\">Declaro ciência da notificação por omissão de prestar contas do(s) Termo(s) listado(s) acima.</label> </div>";
                
                    $strRetorno .= "</div>";
            } else {

                $strRetorno .= "
                    <p style=\"margin: 10px 0px;\"> 
Existe Notificação Eletrônica encaminhada ao Dirigente máximo da Entidade ".$arrParams['dadosUnidade']['dadosUnidade']['entnome']." que carece de leitura e ciência.
Apenas o destinatário pode acessar o documento, por meio do seu login e senha.
Tão logo o Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC verifique a ciência à citada Notificação Eletrônica, os acessos dos demais usuários da Entidade serão reestabelecidos .
            </p>
                    ";
                $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                $strRetorno .= "</div>";
            }
            
          
           
            
            $strRetorno .= "
				<style>
		            .box.box-small {
		                width: 30.3%;
		            }
	
		            .box.box-medium {
		                width: 47%;
		            }
	
		            .box.box-large {
		                width: 97%;
		            }
	
		            .box {
		                FONT: 11pt Arial;
		                -moz-border-radius: 20px;
		                border-radius: 20px;
		                padding: 10px;
		                margin: 10px;
		                float: left;
		            }
	
		            .box .box-header {
		                text-align: center;
		                color: #FFFFFF;
		                height: 30px;
		                font-weight: bold;
		                font-size: 14px;
		            }
	
		            .box .box-header .box-header-options{
		                cursor: pointer;
		                float: right;
		                margin: 0 8px 0 0;
		            }
	
		            .box .box-body {
						text-indent: 10px;
		                text-align: left;
		                background-color: #FFFFFF;
		                border-radius: 20px;
		                border-radius: 5px;
		                padding: 4px;
		                min-height: 130px;
		            }
		            .box .box-body .box-body-title {
		                font-weight: bold;
		                font-size: 14px;
		            }
		            .box .box-body .box-body-subtitle {
		                font-size: 11px;
		            }
	
		            .box.box-red {
		                background-color: #EE3B3B;
		            }
	
		            .box.box-black {
		                background-color: #000000;
		            }
	
		            .box.box-yellow {
		                background-color: #FFC200;
		            }
	
		            .box.box-green {
		                background-color: #348300;
		            }
	
		            .box.box-purple {
		                background-color: #6900AF;
		            }
	
		            .box.box-blue {
		                background-color: #3871C8;
		            }
	
		            .box.box-orange {
		                background-color: #FF8500;
		            }
		            .box p {
		                margin: 0;
		                padding: 0;
		            }
		            .print{
		                background-color: #FFF;
		                padding: 1px;
		                border-bottom: 1px solid #000;
		                border-right: 1px solid #000;
		                border-top: 1px solid #CCC;
		                border-left: 1px solid #CCC;
		            }
				</style>";
         
            
            echo $strRetorno;
            
        }
    }
    /**
     * $ropid = 1 Acesso do gestor ou equipe, 2 = acesso Presidente CACS ou equipe
     * */
    function getAvisoOmissaoPCCACS($arrParams = array(), $ropid = NULL)
    {
        
        if ((count($arrParams) > 0) && ($ropid != NULL)) {
    
            $responsavel = $arrParams['responsavel'];
    
            $strRetorno = "<div id=\"dialog_omissao_ente\" style=\"display: none;\">";
    
            $responsavelEntidadePar = ($arrParams['dadosUnidade']['dadosInstrumento']['itrid'] == 1) ? 'Secretário(a) Estadual de Educação' : 'Prefeito(a)'; //@todo
    
            if ( $responsavel ) {
                // Texto PREFEITO
                if( $ropid == 1 ) {
                    $strRetorno .= '<p style="margin: 10px 0px;"> Senhor(a) '.$responsavelEntidadePar.',</p>';
                    $strRetorno .= '<p style="margin: 10px 0px;"><B>Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC registro de envio do parecer da(s) prestação(ões) de contas relativa ao(s) Termo(s) de Compromisso abaixo identificado(s):</b></p>';
        
                    $strRetorno .= '<div style="clear: both"></div>';
        
                    if (count($arrParams) == 1) {
                        $size = 'large';
                    }
        
                    if (count($arrParams) == 2) {
                        $size = 'medium';
                    }
        
                    if (count($arrParams) == 3) {
                        $size = 'small';
                    }
        
                    $strRetorno .= "<table align=\"center\" class=\"tabela\">
    			    <thead>
    			    	<tr>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Termo de Compromisso</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Processo</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Convenente</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">CNPJ</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Prazo para envio da Análise do CACS</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Início  Fim Vigência</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Valor total repassado</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Programa</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Saldo</td>
    			    	</tr>
    			    </thead>
    			    <tbody>";
        
                    $processos = ($arrParams['arrRegistros']) ? $arrParams['arrRegistros'] : Array();
                    if (count($processos) > 0) {
                        foreach ($processos as $k => $processo) {
        
                            // $strRetorno .= $processo ['numeroprocesso'];
                            $strRetorno .= "
                            <tr>
                            <td align=\"center\"  style=\"FILTER:;padding:5px;\">{$processo['numero_termo']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['processo']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['nome_entidade']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['cnpj_entidade']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_prestar_contas']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_vigencia']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['valor_repassado'])."</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['programa']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['saldo'])."</td>
                    			    	</tr>
                    			    	";
                        }
                    }
        
                    $strRetorno .= "
    			    </tbody>
    			    </table>";
        
                    $strRetorno .= '<p style="margin: 10px 0px;"> A obrigação do Conselho de Controle e Acompanhamento Social - CACS de acompanhar e manifestar-se em relação as prestações de contas de termos de compromisso no âmbito do Plano de Ações Articulas  PAR está prevista nos termos dos Art. 24 da Lei nº 11.494/2007 c/c art. 10 da Lei nº 12.695/12 e demais normativos aplicáveis.</p>';
        
                    $strRetorno .= '<p style="margin: 10px 0px;">Com efeito, esgotado o referido prazo sem envio do parecer, concedemos àquele CACS o prazo de 15 (quinze) dias, a contar da data da ciência deste Ofício, para o saneamento da pendência.</p>';
                    $strRetorno .= '<p style="margin: 10px 0px;">Dessa forma, sugerimos a Vossa Excelência a adoção de medidas cabíveis para fins de saneamento da referida pendência, sob pena de suspensão de repasses de recursos do programa ou projeto, conforme estabelece o § 3º do art. 1º da Resolução CD/FNDE nº 22/2014.</p>';
                    $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                    $strRetorno .= "Equipe PAR MEC/FNDE</p>";
        
                     
                } else if( $ropid == 2 ) {
                     $strRetorno .= '<p style="margin: 10px 0px;"> Senhor(a) Presidente,</p>';
                    $strRetorno .= '<p style="margin: 10px 0px;">Informamos que não consta no Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC registro de envio do parecer da(s) prestação(ões) de contas relativa ao(s) Termo(s) de Compromisso abaixo identificado(s):</p>';
        
                    $strRetorno .= '<div style="clear: both"></div>';
        
                    if (count($arrParams) == 1) {
                        $size = 'large';
                    }
        
                    if (count($arrParams) == 2) {
                        $size = 'medium';
                    }
        
                    if (count($arrParams) == 3) {
                        $size = 'small';
                    }
        
                    $strRetorno .= "<table align=\"center\" class=\"tabela\">
    			    <thead>
    			    	<tr>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Termo de Compromisso</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Processo</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Convenente</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">CNPJ</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Prazo para prestar Contas</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Início  Fim Vigência</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Valor total repassado</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Programa</td>
    			    		<td align=\"center\" bgcolor=\"#e9e9e9\" style=\"FILTER:;padding:5px;\">Saldo</td>
    			    	</tr>
    			    </thead>
    			    <tbody>";
        
                    $processos = ($arrParams['arrRegistros']) ? $arrParams['arrRegistros'] : Array();
                    if (count($processos) > 0) {
                        foreach ($processos as $k => $processo) {
        
                            // $strRetorno .= $processo ['numeroprocesso'];
                            $strRetorno .= "
                            <tr>
                            <td align=\"center\"  style=\"FILTER:;padding:5px;\">{$processo['numero_termo']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['processo']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['nome_entidade']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['cnpj_entidade']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_prestar_contas']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['prazo_vigencia']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['valor_repassado'])."</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">{$processo['programa']}</td>
                            <td align=\"center\" style=\"FILTER:;padding:5px;\">".formata_valor($processo['saldo'])."</td>
                    			    	</tr>
                    			    	";
                        }
                    }
        
                    $strRetorno .= "
    			    </tbody>
    			    </table>";
        
                    $strRetorno .= '<p style="margin: 10px 0px;">A obrigação do Conselho de Controle e Acompanhamento Social - CACS de acompanhar e manifestar-se em relação as prestações de contas de termos de compromisso no âmbito do Plano de Ações Articulas  PAR está prevista nos termos dos Art. 24 da Lei nº 11.494/2007 c/c art. 10 da Lei nº 12.695/12 e demais normativos aplicáveis.</p>';
        
                    $strRetorno .= '<p style="margin: 10px 0px;">Com efeito, esgotado o referido prazo sem envio do parecer, concedemos o prazo de 15 (quinze) dias, a contar da data da ciência deste Ofício, para o saneamento da pendência.</p>';
                    
                    $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                    $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                }
    
                $strRetorno .= "
				    <div align='center' ><input type=\"checkbox\"
	                                                                                         onclick=\"liberaBotao()\"
	                                                                                         name=\"checkbox_ciencia_omissao\" id=\"checkbox_ciencia_omissao\"
	                                                                                         onclick=\"\"
	                                                                                         >&nbsp;
	                        <label style=\"font-size: 12px\">Declaro ciência da notificação por omissão do Controle Social do(s) Termo(s) listado(s) acima.</label> </div>";
    
                $strRetorno .= "</div>";
            } else {
    
                
                if( $ropid == 1 ) {
                    $strRetorno .= "
                    <p style=\"margin: 10px 0px;\">
Existe Notificação Eletrônica encaminhada ao Dirigente máximo da Entidade ".$arrParams['dadosUnidade']['dadosUnidade']['entnome']." que carece de leitura e ciência.
Apenas o destinatário pode acessar o documento, por meio do seu login e senha.
Tão logo o Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC verifique a ciência à citada Notificação Eletrônica, os acessos dos demais usuários da Entidade serão reestabelecidos .
            </p>
                    ";
                    $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                    $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                    $strRetorno .= "</div>";
                } else if( $ropid == 2 ) {
                    $strRetorno .= "
                    <p style=\"margin: 10px 0px;\">
Existe Notificação Eletrônica encaminhada ao Presidente do Controle Social - CACS da Entidade ".$arrParams['dadosUnidade']['dadosUnidade']['entnome']." que carece de leitura e ciência.
Apenas o destinatário pode acessar o documento, por meio do seu login e senha.
Tão logo o Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação - SIMEC verifique a ciência à citada Notificação Eletrônica, os acessos dos demais usuários da Entidade serão reestabelecidos .
            </p>
                    ";
                    $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
                    $strRetorno .= "Equipe PAR MEC/FNDE</p>";
                    $strRetorno .= "</div>";
                }
                
            }
    
    
             
    
            $strRetorno .= "
				<style>
		            .box.box-small {
		                width: 30.3%;
		            }
    
		            .box.box-medium {
		                width: 47%;
		            }
    
		            .box.box-large {
		                width: 97%;
		            }
    
		            .box {
		                FONT: 11pt Arial;
		                -moz-border-radius: 20px;
		                border-radius: 20px;
		                padding: 10px;
		                margin: 10px;
		                float: left;
		            }
    
		            .box .box-header {
		                text-align: center;
		                color: #FFFFFF;
		                height: 30px;
		                font-weight: bold;
		                font-size: 14px;
		            }
    
		            .box .box-header .box-header-options{
		                cursor: pointer;
		                float: right;
		                margin: 0 8px 0 0;
		            }
    
		            .box .box-body {
						text-indent: 10px;
		                text-align: left;
		                background-color: #FFFFFF;
		                border-radius: 20px;
		                border-radius: 5px;
		                padding: 4px;
		                min-height: 130px;
		            }
		            .box .box-body .box-body-title {
		                font-weight: bold;
		                font-size: 14px;
		            }
		            .box .box-body .box-body-subtitle {
		                font-size: 11px;
		            }
    
		            .box.box-red {
		                background-color: #EE3B3B;
		            }
    
		            .box.box-black {
		                background-color: #000000;
		            }
    
		            .box.box-yellow {
		                background-color: #FFC200;
		            }
    
		            .box.box-green {
		                background-color: #348300;
		            }
    
		            .box.box-purple {
		                background-color: #6900AF;
		            }
    
		            .box.box-blue {
		                background-color: #3871C8;
		            }
    
		            .box.box-orange {
		                background-color: #FF8500;
		            }
		            .box p {
		                margin: 0;
		                padding: 0;
		            }
		            .print{
		                background-color: #FFF;
		                padding: 1px;
		                border-bottom: 1px solid #000;
		                border-right: 1px solid #000;
		                border-top: 1px solid #CCC;
		                border-left: 1px solid #CCC;
		            }
				</style>";
             
    
            echo $strRetorno;
    
        }
    }
    
    /*
     * Parametros do array:
     * array(
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * array( 'titulo' => "titulo do aviso", 'texto' => 'texto do aviso' ),
     * ..................
     * )
     *
     */
    function getAvisos($arrAvisos = array(), $adm = false)
    {
        $arrayCores = array(
            '#FF6D3F',
            '#FF9B00',
            '#FFDA00',
            '#95D641',
            '#1CE8B5',
            '#3FC3FF',
            '#B8C4C9'
        );
        
        if (count($arrAvisos) > 0) {
            $strRetorno = "<div id=\"dialog_vigencia\" style=\"display: none;\">";
            $strRetorno .= "<h4>Prezado(a),</h4>";
            $strRetorno .= '<p style="margin: 10px 0px;">Os quadros abaixo apresentam os termos com problema no sistema.</p>';
            $strRetorno .= '<div style="clear: both"></div>';
            
            if (count($arrAvisos) == 1) {
                $size = 'large';
            }
            
            if (count($arrAvisos) == 2) {
                $size = 'medium';
            }
            
            if (count($arrAvisos) == 3) {
                $size = 'small';
            }
            
            foreach ($arrAvisos as $k => $aviso) {
                $class = 'box-black';
                
                if (isset($aviso['status'])) {
                    if ($aviso['status'] == E_ERROR) {
                        $class = 'box-red';
                    } else 
                        if ($aviso['status'] == E_WARNING || $aviso['status'] == E_NOTICE) {
                            $class = 'box-orange';
                        } else {
                            $class = 'box-green';
                        }
                }
                
                $strRetorno .= '<div class="box ' . $class . ' box-' . $size . '">';
                $strRetorno .= '	<div class="box-header">' . strtoupper($aviso['titulo']) . '</div>';
                $strRetorno .= '	<div class="box-body">';
                $strRetorno .= '		<p class="box-body-subtitle"></p>';
                $strRetorno .= '		<p>' . $aviso['texto'] . '</p>';
                $strRetorno .= '	</div>';
                $strRetorno .= '	<div class="box-footer"></div>';
                $strRetorno .= '</div>';
            }


            $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
            $strRetorno .= "Equipe PAR MEC/FNDE</p>";
            $strRetorno .= "</div>";
            
            $strRetorno .= "
				<style>
		            .box.box-small {
		                width: 30.3%;
		            }
		
		            .box.box-medium {
		                width: 47%;
		            }
		
		            .box.box-large {
		                width: 97%;
		            }
		
		            .box {
		                FONT: 11pt Arial;
		                -moz-border-radius: 20px;
		                border-radius: 20px;
		                padding: 10px;
		                margin: 10px;
		                float: left;
		            }
		
		            .box .box-header {
		                text-align: center;
		                color: #FFFFFF;
		                height: 30px;
		                font-weight: bold;
		                font-size: 14px;
		            }
		
		            .box .box-header .box-header-options{
		                cursor: pointer;
		                float: right;
		                margin: 0 8px 0 0;
		            }
		
		            .box .box-body {
						text-indent: 10px;
		                text-align: left;
		                background-color: #FFFFFF;
		                border-radius: 20px;
		                border-radius: 5px;
		                padding: 4px;
		                min-height: 130px;
		            }
		            .box .box-body .box-body-title {
		                font-weight: bold;
		                font-size: 14px;
		            }
		            .box .box-body .box-body-subtitle {
		                font-size: 11px;
		            }
		
		            .box.box-red {
		                background-color: #EE3B3B;
		            }
		
		            .box.box-black {
		                background-color: #000000;
		            }
		
		            .box.box-yellow {
		                background-color: #FFC200;
		            }
		
		            .box.box-green {
		                background-color: #348300;
		            }
		
		            .box.box-purple {
		                background-color: #6900AF;
		            }
		
		            .box.box-blue {
		                background-color: #3871C8;
		            }
		
		            .box.box-orange {
		                background-color: #FF8500;
		            }
		            .box p {
		                margin: 0;
		                padding: 0;
		            } 
		            .print{
		                background-color: #FFF;
		                padding: 1px;
		                border-bottom: 1px solid #000;
		                border-right: 1px solid #000;
		                border-top: 1px solid #CCC;
		                border-left: 1px solid #CCC;
		            }
				</style>";
            
            if (! $adm) {
                $strRetorno .= "			
				 	<script type=\"text/javascript\">
				        jQuery(function(){
		                    jQuery(\"#dialog_vigencia\").dialog({
		                        modal: true,
		                        width: '90%',
								height: 700,
		                        open: function(){
						            jQuery('.ui-widget-overlay').bind('click',function(){
						                jQuery('#dialog_vigencia').dialog('close');
						            })
						        },
		                        buttons: {
		                            Fechar: function() {
		                                jQuery(\"#dialog_detalhe_processo\").html('');
		                                jQuery( this ).dialog( \"close\" );
		                            }
		                        }
		                    });
				        });
				    </script>";
            }
            
            echo $strRetorno;
        }
    }

    function getAvisosCacsPar($arrAvisos = array(), $adm = false)
    {
        $arrayCores = array(
            '#FF6D3F',
            '#FF9B00',
            '#FFDA00',
            '#95D641',
            '#1CE8B5',
            '#3FC3FF',
            '#B8C4C9'
        );

        if (count($arrAvisos) > 0) {
            $strRetorno = "<div id=\"dialog_vigencia\" style=\"display: none;\">";
            $strRetorno .= "<h4>Prezado(a),</h4>";
            $strRetorno .= '<p style="margin: 10px 0px;">Os quadros abaixo apresentam termos pendentes de analise do Conselho e tutoriais para emissão do parecer.</p>';
            $strRetorno .= '<div style="clear: both"></div>';


            if (count($arrAvisos) == 1) {
                $size = 'large';
            }

            if (count($arrAvisos) == 2) {
                $size = 'medium';
            }

            foreach ($arrAvisos as $k => $aviso) {
                $class = 'box-black';

                if (isset($aviso['status'])) {
                    if ($aviso['status'] == E_ERROR) {
                        $class = 'box-red';
                    } else
                        if ($aviso['status'] == E_WARNING || $aviso['status'] == E_NOTICE) {
                            $class = 'box-orange';
                        } else {
                            $class = 'box-green';
                        }
                }

                $strRetorno .= '<div class="box ' . $class . ' box-' . $size . '">';
                $strRetorno .= '	<div class="box-header">' . strtoupper($aviso['titulo']) . '</div>';
                $strRetorno .= '	<div class="box-body">';
                $strRetorno .= '		<p class="box-body-subtitle"></p>';
                $strRetorno .= '		<p>' . $aviso['texto'] . '</p>';
                $strRetorno .= '	</div>';
                $strRetorno .= '	<div class="box-footer"></div>';
                $strRetorno .= '</div>';
            }


            $strRetorno .= "<div style='clear: both'></div><p>Atenciosamente.<br>";
            $strRetorno .= "Equipe PAR MEC/FNDE</p>";
            $strRetorno .= "<br/><br/><div style='clear: both; color: red; font-size: 17px' align='center'><p>
                            *Esclarecemos que as ações do PAC não requerem manifestação dos Conselhos de Acompanhamento e Controle Social.
                            </p></div><br/>";
            $strRetorno .= "</div>";


            $strRetorno .= "
				<style>
		            .box.box-small {
		                width: 30.3%;
		            }
		
		            .box.box-medium {
		                width: 47%;
		            }
		
		            .box.box-large {
		                width: 97%;
		            }
		
		            .box {
		                FONT: 11pt Arial;
		                -moz-border-radius: 20px;
		                border-radius: 20px;
		                padding: 10px;
		                margin: 10px;
		                float: left;
		            }
		
		            .box .box-header {
		                text-align: center;
		                color: #FFFFFF;
		                height: 30px;
		                font-weight: bold;
		                font-size: 14px;
		            }
		
		            .box .box-header .box-header-options{
		                cursor: pointer;
		                float: right;
		                margin: 0 8px 0 0;
		            }
		
		            .box .box-body {
						text-indent: 10px;
		                text-align: left;
		                background-color: #FFFFFF;
		                border-radius: 20px;
		                border-radius: 5px;
		                padding: 4px;
		                min-height: 130px;
		            }
		            .box .box-body .box-body-title {
		                font-weight: bold;
		                font-size: 14px;
		            }
		            .box .box-body .box-body-subtitle {
		                font-size: 11px;
		            }
		
		            .box.box-red {
		                background-color: #EE3B3B;
		            }
		
		            .box.box-black {
		                background-color: #000000;
		            }
		
		            .box.box-yellow {
		                background-color: #FFC200;
		            }
		
		            .box.box-green {
		                background-color: #348300;
		            }
		
		            .box.box-purple {
		                background-color: #6900AF;
		            }
		
		            .box.box-blue {
		                background-color: #3871C8;
		            }
		
		            .box.box-orange {
		                background-color: #FF8500;
		            }
		            .box p {
		                margin: 0;
		                padding: 0;
		            } 
		            .print{
		                background-color: #FFF;
		                padding: 1px;
		                border-bottom: 1px solid #000;
		                border-right: 1px solid #000;
		                border-top: 1px solid #CCC;
		                border-left: 1px solid #CCC;
		            }
				</style>";

            if (! $adm) {
                $strRetorno .= "			
				 	<script type=\"text/javascript\">
				        jQuery(function(){
		                    jQuery(\"#dialog_vigencia\").dialog({
		                        modal: true,
		                        width: '90%',
								height: 700,
		                        open: function(){
						            jQuery('.ui-widget-overlay').bind('click',function(){
						                jQuery('#dialog_vigencia').dialog('close');
						            })
						        },
		                        buttons: {
		                            Fechar: function() {
		                                jQuery(\"#dialog_detalhe_processo\").html('');
		                                jQuery( this ).dialog( \"close\" );
		                            }
		                        }
		                    });
				        });
				    </script>";
            }

            echo $strRetorno;
        }
    }
}