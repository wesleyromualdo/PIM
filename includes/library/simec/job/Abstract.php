<?php

/**
 * Class Simec_Job_Abstract
 * $Id: Abstract.php 135332 2017-12-11 18:32:44Z saulocorreia $
 */
abstract class Simec_Job_Abstract extends Simec_Job_Model
{
    const SIMEC_JOB_SESSION = 'simec_job_session';
    /**
     * @var array
     */
    protected static $predecessor = [];

    /**
     * @var array
     */
    protected static $predecessorParams = [];
    /**
     * @var string
     */
    protected $baseUrl;
    /**
     * @var array
     */
    private $params = [];

    /**
     * Simec_Job_Abstract constructor.
     */
    public function __construct()
    {
        $this->params = array_merge(
            $this->params,
            $this->loadParams()
        );
    }

    /**
     * @param string $jobClassName
     * @param array  $predecessorParams
     *
     * @throws Simec_Job_Exception
     */
    public static function setPredecessor($jobClassName, $predecessorParams = [])
    {
        if (!(new ReflectionClass($jobClassName))->isSubclassOf(Simec_Job_Abstract::class)) {
            throw new Simec_Job_Exception(
                "{$jobClassName} não é uma subclasse de " . Simec_Job_Abstract::class
            );
        }

        static::$predecessor[get_called_class()] = $jobClassName;
        static::$predecessorParams[get_called_class()] = array_merge(self::getSessao(), $predecessorParams);
    }

    /**
     * @return string
     */
    public static function getPredecessor()
    {
        return static::$predecessor[get_called_class()];
    }

    /**
     * @return array
     */
    public static function getPredecessorParams()
    {
        return static::$predecessorParams[get_called_class()];
    }

    public final static function run()
    {
        set_error_handler(function () {
            return true;
        });

        $jobId = ZendJobQueue::getCurrentJobId();

        $erro = '';

        $class = get_called_class();
        $job = new $class();

        try {
            $job->init();
            $job->execute();

            // preencher status
            ZendJobQueue::setCurrentJobStatus(ZendJobQueue::OK);
            $status = ZendJobQueue::STATUS_OK;
        } catch (Exception $e) {
            $erro = $e->getMessage();

            // preencher status
            ZendJobQueue::setCurrentJobStatus(ZendJobQueue::FAILED, $erro);

            $status = ZendJobQueue::STATUS_LOGICALLY_FAILED;
        } finally {
            $job->shutdown();
        }

        try {
            (new Simec_Job_Controller_Joblog())->atualizarFimJob($jobId, $status, $erro, self::converterParametros()['Simec_Job_Host']);
        } catch (Exception $e) {

        }
    }

    /**
     * @return array
     */
    private static function getSessao()
    {
        $sessao = $_SESSION;

        unset($sessao['acl']);
        unset($sessao['perfil']);
        unset($sessao['perfils']);
        unset($sessao['perfilusuario']);
        unset($sessao['simec-listagem']);
        unset($sessao['sistemas']);
        unset($sessao['tipodocumentos']);

        return [self::SIMEC_JOB_SESSION => $sessao];
    }

    /**
     * @return array
     */
    private static function converterParametros()
    {
        return simec_utf8_decode_recursive_and_key(ZendJobQueue::getCurrentJobParams());
    }

    /**
     * Retorna o label dada para este Job
     *
     * @return string
     */
    public abstract function getName();

    public function salvarOutput()
    {
        (new Simec_Job_Controller_Joblog())->atualizarOutput(ZendJobQueue::getCurrentJobId(), $this->loadParams()['Simec_Job_Host']);
    }

    /**
     * @param      $sources
     * @param bool $removeFiles
     *
     * @return bool|string
     * @throws Exception
     */
    protected function zipFile($sources, $removeFiles = false)
    {
        try {
            if (!is_array($sources)) {
                $sources = [$sources];
            }

            $filename = tempnam(sys_get_temp_dir(), 'saida');

            $zip = new ZipArchive();
            if($zip->open($filename, ZipArchive::OVERWRITE) !== true) {
                if($zip->open($filename, ZipArchive::CREATE) !== true) {
                    return false;
                }
            }

            foreach ($sources as $source) {
                $source = str_replace('\\', '/', $source);
                if (!file_exists($source)) {
                    continue;
                }

                if (is_dir($source) === true) {
                    $files = new RecursiveIteratorIterator(new RecursiveDirectoryIterator($source), RecursiveIteratorIterator::SELF_FIRST);

                    foreach ($files as $file) {
                        $file = str_replace('\\', '/', $file);

                        // Ignore "." and ".." folders
                        if (in_array(substr($file, strrpos($file, '/') + 1), ['.', '..']) || "{$source}" == $file) {
                            continue;
                        }

                        if (is_dir($file) === true) {
                            $zip->addEmptyDir(str_replace($source . '/', '', $file . '/'));
                        } elseif (is_file($file) === true) {
                            $zip->addFromString(str_replace($source . '/', '', $file), file_get_contents(realpath($file)));
                        }
                    }
                } elseif (is_file($source) === true) {
                    $zip->addFromString(basename($source), file_get_contents($source));
                }

                if ($removeFiles) {
                    unlink($source);
                }
            }
            $zip->close();

            $conteudo = file_get_contents($filename);

            unlink($filename);

            return $conteudo;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @return array
     * @throws Exception
     */
    protected function loadParams()
    {
        try {
            $parametros = self::converterParametros();

            unset($parametros[self::SIMEC_JOB_SESSION]);

            return $parametros;
        } catch (Exception $e) {
            throw $e;
        }
    }

    /**
     * @param string    $metodo
     * @param           $retornoSoap
     * @param string    $esquema
     * @param Exception $exception
     *
     * @return Exception
     */
    protected function gerarXml($metodo, $retornoSoap, $esquema, $exception)
    {
        try {
            if (empty($esquema)) {
                return $exception;
            }

            $model = new Simec_Job_Model_Logws($esquema);
            $model->getLastItem($metodo);

            $dir = sys_get_temp_dir() . "/" . uniqid() . "/";

            mkdir($dir, "777");

            $tmpRequest = $this->gerarArquivoXml('Request', $dir, $model->lwsrequestcontent);
            $tmpResponse = $this->gerarArquivoXml('Response', $dir, $model->lwsresponsecontent);

            $conteudo = $this->zipFile([$tmpRequest, $tmpResponse], true);

            $sessao = self::converterParametros()[self::SIMEC_JOB_SESSION];

            $_SESSION["sisid"] = $sessao['sisid'];
            $arquivo = new FilesSimec(null, null, $sessao['sisdiretorio']);
            $arqid = $arquivo->setStream('Falha com WS SIOP', $conteudo, 'application/zip', '.zip', false, 'Falha WS ' . date('Y-m-d His') . '.zip');

            if (empty($arqid)) {
                $html = ob_get_contents();
            } else {
                $html = <<<HTML
Erro: {$exception->getMessage()}<br><br>Faça o Download do arquivo e abra um chamado no Suporte SIOP.<br><br>
<button class="btn btn-danger dim" onclick="SimecJobDownloadErro({$arqid}, '{$esquema}')">Download Arquivo</button>
HTML;
            }

            return new Exception($html, 0, $exception);
        } catch (Exception $e) {
            return new Exception($e->getMessage(), 0, $exception);
        }
    }

    protected abstract function init();

    protected abstract function execute();

    protected abstract function shutdown();

    /**
     * @param $prefix
     * @param $conteudo
     *
     * @return bool|string
     */
    protected function gerarArquivoXml($nome, $dir, $conteudo)
    {
        try {
            $nomeArquivo = "{$dir}{$nome}.xml";

            $handle = fopen($nomeArquivo, 'w');

            fwrite($handle, $conteudo);

            fclose($handle);

            return $nomeArquivo;
        } catch (Exception $e) {
            throw $e;
        }

    }
}
