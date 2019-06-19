<?php

switch ($requisicao) {
    case 'baixar-arquivo':
        $file = new FilesSimec();
        $file->setEsquema('emenda');
        $arqid = $_REQUEST['arqid'];
        $arquivo = $file->getDownloadArquivo($arqid,'emendas');

//        ver($_REQUEST,$arquivo,d);

        break;
    case 'apagar-anexo':
        #ver($_REQUEST['arqidDel'], d);
        $sql = "DELETE FROM emenda.anexo where arqid=" . $_REQUEST['arqidDel'];
        $db->executar($sql);
        $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=" . $_REQUEST['arqidDel'];
        $db->executar($sql);
        $db->commit();
        $file = new FilesSimec();
        $file->excluiArquivoFisico($_REQUEST['arqidDel']);
        $fm->addMensagem('Arquivo apagado com sucesso.');

        $_SERVER['REQUEST_URI'] = preg_replace('|&requisicao=apagar-anexo&arqidDel=\d+|', '', $_SERVER['REQUEST_URI']);

        break;
    case 'consultar-impedimentos':
        echo simec_json_encode(
                (new Spoemendas_Model_Emendaimpedimentobeneficiario())
                        ->pegaUmImpedimento($_REQUEST['dados'])
        );
        die();
    case 'salvar-impedimentos':
        $dados = $_REQUEST['impedimentos'];
        $dados['edivalor'] != ''? $dados['edivalor'] : $dados['edivalor'] = 0;
        // Verifica se a emenda não ha impedimento se não houve limpa o valor
        $dados['ediimpositivo'] == 'NH'? $dados['edivalor'] = 0 : $dados['edivalor'];
        $emDetImpositivo = new Spoemendas_Model_Emendadetalheimpositivo();
        $emDetImpositivo->popularDadosObjeto($dados);

        $validado = true;
        if ($dados['ediimpositivo'] == 'TO') {
            if ($dados['valor_entidade'] != MoedaToBd($dados['edivalor'])) {
                $validado = false;
                $fm->addMensagem('Valor de impedimento inserido não é igual ao valor total da entidade beneficiada.', Simec_Helper_FlashMessage::ERRO);
            }
        } elseif ($dados['ediimpositivo'] == 'PA') {
            if ($dados['valor_entidade'] <= MoedaToBd($dados['edivalor'])) {
                $validado = false;
                $fm->addMensagem('Valor de impedimento inserido é igual ou superior ao valor total da entidade beneficiada.', Simec_Helper_FlashMessage::ERRO);
            }

            if (MoedaToBd($dados['edivalor']) == 0) {
                $validado = false;
                $fm->addMensagem('Valor de impedimento deve ser maior que 0 (zero).', Simec_Helper_FlashMessage::ERRO);
            }
        } else {
            $emDetImpositivo->edivalor = '0';
        }

        if ($dados['emiid'] == 6 && empty($dados['edidescricao']))
        {
            $validado = false;
            $fm->addMensagem('O campo Justificativa é de preenchimento obrigatório', Simec_Helper_FlashMessage::ERRO);
        }

        if ($validado)
        {
            if ($emDetImpositivo->salvar())
            {
                $emDetImpositivo->commit();

                if (empty($dados['ediid']))
                {
                    $dados['ediid'] = $emDetImpositivo->ediid;
                }

                /*
                 *  Salvar o Arquivo do anexo
                 */
                if ($_FILES["impedimentos_arquivo"]['size'] && ($_FILES["impedimentos_arquivo"]['error']==0))
                {
                    $validTypes = ['doc', 'docx', 'odt', 'png', 'jpg', 'jpeg', 'pdf'];
                    $fileInfo = pathinfo($_FILES["impedimentos_arquivo"]["name"]);

                    if (in_array(strtolower($fileInfo['extension']), $validTypes)) {
                        $campos = [
                            "anxdsc"  => "'{$dados['anxdescricao']}'",
                            "anxdata" => "'now()'",
                            "ediid"   => $dados['ediid']
                        ];
                        $file = new FilesSimec("anexo", $campos, "emenda");
                        $arquivoSalvo = $file->setUpload();
                        $fm->addMensagem('Arquivo salvo com sucesso.');
                    } else {
                        $fm->addMensagem('Tipo de arquivo enviado é inválido. Tipos aceitos: ' .implode(', ', $validTypes) . '.', Simec_Helper_FlashMessage::ERRO);
                    }
                }
                if (!empty($dados['emiid']))
                {
                    $emImpBeneficiario = new Spoemendas_Model_Emendaimpedimentobeneficiario();
                    $emImpBeneficiario->popularDadosObjeto($dados);

                    if ($emImpBeneficiario->salvar())
                    {
                        $emImpBeneficiario->commit();

                        $fm->addMensagem('Impedimento / Plano de Trabalho salvo com sucesso.');
                    }
                    else
                    {
                        $fm->addMensagem('Não foi possível realizar a requisição solicitada.', Simec_Helper_FlashMessage::ERRO);
                    }
                }
            }
            else
            {
                $fm->addMensagem('Não foi possível realizar a requisição solicitada.', Simec_Helper_FlashMessage::ERRO);
            }
        }
}

header("Location: {$_SERVER['REQUEST_URI']}");
die();
